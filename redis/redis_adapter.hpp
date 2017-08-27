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
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <json/json.hpp>
#include <mutex>
#include <shared_mutex>

#include "remote_server.hpp"

using json = nlohmann::json;

namespace ws{
class redis_adapter {
  struct connection_hdl_key_map_comparator{
    bool operator()(const websocketpp::connection_hdl &lhs, const websocketpp::connection_hdl &rhs){
      return lhs.owner_before(rhs);
    }
  };

  remote_server remote;

  std::string server_id;
  server* s;
  std::map<std::string, websocketpp::connection_hdl> active_connections;
  std::shared_mutex connections_mutex;
  std::map<websocketpp::connection_hdl, std::string, connection_hdl_key_map_comparator> active_connections_back_pointer;
  std::mutex back_pointer_mutex;
  std::map<std::string, websocketpp::connection_hdl>::iterator find_by_conn(websocketpp::connection_hdl hdl);
  boost::uuids::random_generator uuid_generator;
  //TODO: This is not thread safe

  void safe_send(const std::string &id, const std::string &from_id, const std::string &msg);
public:
  explicit redis_adapter(std::string id) : server_id(id), remote(remote_server(std::bind(&redis_adapter::on_internal_message, this, ::_1, ::_2))){};
  explicit redis_adapter(redis_adapter&& adapter);
  explicit redis_adapter(const redis_adapter& adapter);
  explicit redis_adapter(redis_adapter& adapter);
  void generate_new_id();
  void login(std::string username, std::string password, websocketpp::connection_hdl conn);
  void logout(websocketpp::connection_hdl conn);
  std::vector<std::string> list_connections(websocketpp::connection_hdl conn);
  void send(std::string target, std::string msg, websocketpp::connection_hdl conn);

  void on_internal_message(websocketpp::connection_hdl conn, message_ptr msg);

  void set_server(server* s){
    this->s = s;
  }
};
}


#endif //DISTRIBUTED_WEBSOCKETS_REDIS_ADAPTER_HPP
