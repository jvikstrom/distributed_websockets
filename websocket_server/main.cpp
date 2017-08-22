//
// Created by jovi on 2017-08-21.
//
#include <iostream>
#include <functional>
#include "websocket_server.hpp"
#include "websocket_endpoints.hpp"
#include "endpoints/login.hpp"
#include "endpoints/list_users.hpp"
#include "endpoints/send.hpp"

ws::redis_adapter redis("uuid");
ws::websocket_endpoints endpoints(redis);
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
  std::cout << "on message: " << msg->get_payload() << std::endl;
  endpoints.handle_message(s, std::move(hdl), msg);
}

int main(){
  endpoints.add_endpoint("list", std::make_unique<ws::list_users_endpoint>());
  endpoints.add_endpoint("login", std::make_unique<ws::login_endpoint>());
  endpoints.add_endpoint("send", std::make_unique<ws::send_endpoint>());
  ws::websocket_server_config conf(8010, on_message);
  ws::websocket_server server(conf);
  server.start();
  std::cout << "Hello" << std::endl;
}