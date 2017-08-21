//
// Created by jovi on 2017-08-21.
//

#ifndef DISTRIBUTED_WEBSOCKETS_WEBSOCKET_SERVER_CONFIG_HPP
#define DISTRIBUTED_WEBSOCKETS_WEBSOCKET_SERVER_CONFIG_HPP

#include <functional>
typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef server::message_ptr message_ptr;

typedef std::function<void(server*, websocketpp::connection_hdl, message_ptr)> on_message_func;

namespace ws{
struct websocket_server_config{
  websocket_server_config(uint16_t port, on_message_func on_message) : port(port), on_message(on_message){}
  uint16_t port;
  on_message_func on_message;
};
}
#endif //DISTRIBUTED_WEBSOCKETS_WEBSOCKET_SERVER_CONFIG_HPP
