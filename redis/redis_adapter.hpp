//
// Created by jovi on 2017-08-22.
//

#ifndef DISTRIBUTED_WEBSOCKETS_REDIS_ADAPTER_HPP
#define DISTRIBUTED_WEBSOCKETS_REDIS_ADAPTER_HPP

#include <iostream>
#include <map>
#include <vector>
#include <websocket_server.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace ws{
class redis_adapter {
  std::string server_id;
  server* s;
  std::map<std::string, websocketpp::connection_hdl> active_connections;
  //TODO: This is not thread safe
public:
  redis_adapter(std::string id) : server_id(id){};
  void login(std::string username, std::string password, websocketpp::connection_hdl conn);
  void logout(websocketpp::connection_hdl conn);
  std::vector<std::string> list_connections(websocketpp::connection_hdl conn);

  void set_server(server* s){
    this->s = s;
  }
};
}


#endif //DISTRIBUTED_WEBSOCKETS_REDIS_ADAPTER_HPP