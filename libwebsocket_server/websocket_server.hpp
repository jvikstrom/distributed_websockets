//
// Created by jovi on 2017-08-21.
//

#ifndef DISTRIBUTED_WEBSOCKETS_WEBSOCKET_SERVER_HPP
#define DISTRIBUTED_WEBSOCKETS_WEBSOCKET_SERVER_HPP

#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <functional>
#include "websocket_server_config.hpp"

// pull out the type of messages sent by our config

namespace ws{
  void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
class websocket_server {
  server _server;
public:
  websocket_server(websocket_server_config& config);
};
}


#endif //DISTRIBUTED_WEBSOCKETS_WEBSOCKET_SERVER_HPP
