//
// Created by jovi on 2017-08-21.
//
#define PROFILE 1
#include <iostream>
#include <functional>
#include <future>
#include "websocket_server.hpp"
#include "websocket_endpoints.hpp"
#include "endpoints/login.hpp"
#include "endpoints/list_users.hpp"
#include "endpoints/send.hpp"

ws::redis_adapter redis("uuid");
ws::websocket_endpoints endpoints(redis);
#ifdef PROFILE
int n_messages = 0, n_messages_session = 0, connected = 0;
#endif
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
  //std::cout << "on message: " << msg->get_payload() << std::endl;
  std::async(std::launch::async, std::bind(&ws::websocket_endpoints::handle_message, &endpoints, s, std::move(hdl), msg));
  //endpoints.handle_message(s, std::move(hdl), msg);
#ifdef PROFILE
  n_messages++;
  n_messages_session++;
#endif
}

void on_open(server* s, websocketpp::connection_hdl hdl){
#ifdef PROFILE
  connected++;
  if(connected % 100 == 0){
    std::cout << "Currently: " << connected << " clients" << std::endl;
  }
#endif
}
bool triggered = false;
void on_close(server* s, websocketpp::connection_hdl hdl){
#ifdef PROFILE
  connected--;
  if(connected % 100 == 0){
    std::cout << "Currently: " << connected << " clients" << std::endl;
  }
  if(connected < 100 && !triggered){
    triggered = true;
    std::cout << "There were : " << n_messages_session << " messages sent" << std::endl;
    std::cout << "Total messages during lifetime: " << n_messages << std::endl;
    n_messages_session = 0;
    s->stop_listening();
  }
#endif
}

int main(){
  endpoints.add_endpoint("list", std::make_unique<ws::list_users_endpoint>());
  endpoints.add_endpoint("login", std::make_unique<ws::login_endpoint>());
  endpoints.add_endpoint("send", std::make_unique<ws::send_endpoint>());
  ws::websocket_server_config conf(8010, on_message, on_open, on_close);
  ws::websocket_server server(conf);
  server.start();
  std::cout << "Hello" << std::endl;
}