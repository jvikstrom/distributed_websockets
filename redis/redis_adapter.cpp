//
// Created by jovi on 2017-08-22.
//

#include "redis_adapter.hpp"

void ws::redis_adapter::login(std::string username, std::string password, websocketpp::connection_hdl conn) {
  active_connections[username] = conn;
  std::string payload = R"({"msg": "Successfully logged in!"})";
  s->send(conn, payload, websocketpp::frame::opcode::text);
}

void ws::redis_adapter::logout(websocketpp::connection_hdl conn){
  for(auto it = active_connections.begin(); it != active_connections.end(); ++it){
    if(!it->second.owner_before(conn) && !conn.owner_before(it->second)){
      active_connections.erase(it);
      return;
    }
  }
}

std::vector<std::string> ws::redis_adapter::list_connections(websocketpp::connection_hdl conn) {
  std::vector<std::string> usernames;
  for(auto it = active_connections.begin(); it != active_connections.end(); ++it){
    usernames.push_back(it->first);
  }
  return usernames;
}