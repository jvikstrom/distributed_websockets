//
// Created by jovi on 2017-08-22.
//

#include "redis_adapter.hpp"

ws::redis_adapter::redis_adapter(ws::redis_adapter&& adapter){
  this->server_id = adapter.server_id;
  this->s = adapter.s;
  this->active_connections = adapter.active_connections;
}

ws::redis_adapter::redis_adapter(const ws::redis_adapter& adapter){
  this->server_id = adapter.server_id;
  this->s = adapter.s;
  this->active_connections = adapter.active_connections;
}

ws::redis_adapter::redis_adapter(ws::redis_adapter& adapter){
  this->server_id = adapter.server_id;
  this->s = adapter.s;
  this->active_connections = adapter.active_connections;
}

void ws::redis_adapter::login(std::string username, std::string password, websocketpp::connection_hdl conn) {
  connections_mutex.lock();
  active_connections[username] = conn;
  connections_mutex.unlock();

  std::string payload = R"({"cmd": "login", "msg": "Successfully logged in!"})";
  s->send(conn, payload, websocketpp::frame::opcode::text);
}

void ws::redis_adapter::logout(websocketpp::connection_hdl conn){
  connections_mutex.lock();
  auto it = find_by_conn(conn);
  if(it == active_connections.end()){
    connections_mutex.unlock();
    return;
  }
  active_connections.erase(it);
  connections_mutex.unlock();
}

std::vector<std::string> ws::redis_adapter::list_connections(websocketpp::connection_hdl conn) {
  std::vector<std::string> usernames;
  connections_mutex.lock();
  for(auto it = active_connections.begin(); it != active_connections.end(); ++it){
    usernames.push_back(it->first);
  }
  connections_mutex.unlock();

  return usernames;
}

std::map<std::string, websocketpp::connection_hdl>::iterator ws::redis_adapter::find_by_conn(websocketpp::connection_hdl conn){
  for(auto it = active_connections.begin(); it != active_connections.end(); ++it){
    if(!it->second.owner_before(conn) && !conn.owner_before(it->second)){
      return it;
    }
  }
  return active_connections.end();
}

void ws::redis_adapter::send(std::string target, std::string msg, websocketpp::connection_hdl conn){
  connections_mutex.lock();
  auto it = this->active_connections.find(target);
  if(it == active_connections.end()){
    connections_mutex.unlock();
    std::string payload = R"({"cmd": "error", "msg": "No such user!"})";
    s->send(conn, payload, websocketpp::frame::opcode::text);
    return;
  }
  connections_mutex.unlock();

  auto send_it = find_by_conn(conn);
  json j;
  j["cmd"] = "receive";
  j["msg"] = msg;
  j["from"] = send_it->first;
  std::string payload = j.dump(4);
  s->send(it->second, payload, websocketpp::frame::opcode::text);
}

void ws::redis_adapter::generate_new_id() {
  boost::uuids::uuid uuid = uuid_generator();
  server_id = boost::lexical_cast<std::string>(uuid);
}