//
// Created by jovi on 2017-08-22.
//

#include "redis_adapter.hpp"

ws::redis_adapter::redis_adapter(ws::redis_adapter&& adapter) : remote(adapter.remote){
  this->server_id = adapter.server_id;
  this->s = adapter.s;
  this->active_connections = adapter.active_connections;
  this->active_connections_back_pointer = adapter.active_connections_back_pointer;
}

ws::redis_adapter::redis_adapter(const ws::redis_adapter& adapter) : remote(adapter.remote){
  this->server_id = adapter.server_id;
  this->s = adapter.s;
  this->active_connections = adapter.active_connections;
  this->active_connections_back_pointer = adapter.active_connections_back_pointer;
}

ws::redis_adapter::redis_adapter(ws::redis_adapter& adapter) : remote(adapter.remote){
  this->server_id = adapter.server_id;
  this->s = adapter.s;
  this->active_connections = adapter.active_connections;
  this->active_connections_back_pointer = adapter.active_connections_back_pointer;
}

void ws::redis_adapter::login(std::string username, std::string password, websocketpp::connection_hdl conn) {
  connections_mutex.lock();
  active_connections.insert(std::make_pair(username, conn));
  active_connections_back_pointer.insert(std::make_pair(conn, username));
  connections_mutex.unlock();

  std::string payload = R"({"cmd": "login", "msg": "Successfully logged in!"})";
  s->send(conn, payload, websocketpp::frame::opcode::text);
}

void ws::redis_adapter::logout(websocketpp::connection_hdl conn){
  connections_mutex.lock();
  auto it = find_by_conn(conn);
  if(it != active_connections.end()){
    active_connections.erase(it);
  }

  auto it2 = active_connections_back_pointer.find(conn);
  if(it2 != active_connections_back_pointer.end()){
    active_connections_back_pointer.erase(it2);
  }
  connections_mutex.unlock();
}

std::vector<std::string> ws::redis_adapter::list_connections(websocketpp::connection_hdl conn) {
  std::vector<std::string> usernames;
  connections_mutex.lock_shared();
  for(auto it = active_connections.begin(); it != active_connections.end(); ++it){
    usernames.push_back(it->first);
  }
  connections_mutex.unlock_shared();

  return usernames;
}

std::map<std::string, websocketpp::connection_hdl>::iterator ws::redis_adapter::find_by_conn(websocketpp::connection_hdl conn){
  auto it = active_connections_back_pointer.find(conn);
  if(it == active_connections_back_pointer.end()){
    return active_connections.end();
  }
  return active_connections.find(it->second);
}

void ws::redis_adapter::send(std::string target, std::string msg, websocketpp::connection_hdl conn){
  connections_mutex.lock_shared();
  auto it = find_by_conn(conn);
  if(it == active_connections.end()){
    return;
  }
  connections_mutex.unlock_shared();
  remote.send(target, it->first, msg);

//  safe_send(target, it->first, msg);
  return;

  auto send_it = find_by_conn(conn);
  json j;
  j["cmd"] = "receive";
  j["msg"] = msg;
  j["from"] = send_it->first;
  std::string payload = j.dump();
  try{
    s->send(it->second, payload, websocketpp::frame::opcode::text); //TODO: Wrap all s->send in try/catch blocks
  }catch(std::exception e){
    std::cout << "Exception: " << e.what() << std::endl;
  }

}

void ws::redis_adapter::on_internal_message(websocketpp::connection_hdl conn, message_ptr msg){
  json payload;
  try{
    payload = json::parse(msg->get_payload());
  }catch(std::exception &e){
    std::cout << "Exception on parse inner message: " << e.what() << ". Message payload: " << msg->get_payload() << std::endl;
    return;
  }

  json j;
  try{
    j = json::parse(msg->get_payload());
  }catch(std::exception &e){
    std::cout << "Exception on internal message received: " << e.what() << std::endl;
  }
  if(j["from"] == nullptr || j["id"] == nullptr || j["msg"] == nullptr){
    return;
  }

  safe_send(j["id"].as<std::string>(), j["from"].as<std::string>(), j["msg"].as<std::string>());
}

void ws::redis_adapter::safe_send(const std::string &id, const std::string &from_id, const std::string &msg){
  connections_mutex.lock_shared();
  auto it = this->active_connections.find(id);
  if(it == active_connections.end()){
    return;
  }
  connections_mutex.unlock_shared();

  json j;
  j["cmd"] = "receive";
  j["msg"] = msg;
  j["from"] = from_id;
  std::string payload = j.dump();
  try{
    s->send(it->second, payload, websocketpp::frame::opcode::text);
  }catch(std::exception &e){
    std::cout << "Exception: " << e.what() << std::endl;
  }
}

void ws::redis_adapter::generate_new_id() {
  boost::uuids::uuid uuid = uuid_generator();
  server_id = boost::uuids::to_string(uuid);
}