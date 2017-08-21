//
// Created by jovi on 2017-08-21.
//
#include <iostream>
#include <functional>
#include "websocket_server.hpp"

void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
  std::cout << "on message: " << msg->get_payload() << std::endl;
}

int main(){
  ws::websocket_server_config conf(8010, on_message);
  ws::websocket_server server(conf);
  server.start();
  std::cout << "Hello" << std::endl;
}