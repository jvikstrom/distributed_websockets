//
// Created by jovi on 2017-08-21.
//

#ifndef DISTRIBUTED_WEBSOCKETS_WEBSOCKET_ENDPOINTS_HPP
#define DISTRIBUTED_WEBSOCKETS_WEBSOCKET_ENDPOINTS_HPP

#include <map>
#include <memory>
#include <json/json.hpp>
#include <cpp_redis/cpp_redis>
#include <websocket_server.hpp>
#include <redis_adapter.hpp>
#include "endpoints/base_endpoint.hpp"

namespace ws {
class websocket_endpoints {
  std::map<std::string, std::unique_ptr<base_endpoint>> endpoints;
  ws::redis_adapter adapter;
public:
  explicit websocket_endpoints(ws::redis_adapter adapter) : adapter(std::move(adapter)){}
  void handle_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg);
  void add_endpoint(std::string cmd, std::unique_ptr<base_endpoint> endpoint);
};
}

#endif //DISTRIBUTED_WEBSOCKETS_WEBSOCKET_ENDPOINTS_HPP
