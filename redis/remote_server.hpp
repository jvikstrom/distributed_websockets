//
// Created by jovi on 2017-08-26.
//

#ifndef DISTRIBUTED_WEBSOCKETS_REMOTE_SERVER_HPP
#define DISTRIBUTED_WEBSOCKETS_REMOTE_SERVER_HPP

#include <iostream>
#include <map>
#include <thread>
#include <memory>
#include <shared_mutex>
#include <functional>
#include <websocketpp/client.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <cpp_redis/cpp_redis>
#include "websocket_server_config.hpp"
#include "websocket_server.hpp"

namespace ws{
typedef std::shared_ptr<websocketpp::client<websocketpp::config::asio_client>> websocket_client;
class remote_server {
  const std::string redis_host = "localhost";
  const uint16_t port = 8000;

  cpp_redis::redis_client client;
  std::thread internal_server_thread;
  std::map<std::string, websocket_client> open_clients;
  std::map<std::string, std::thread> open_client_threads;
  std::shared_mutex client_mutex;
  std::unique_ptr<websocket_server> s;
  //websocketpp::server* server;
  on_message_func on_message;

  void open_connection(const std::string& host, std::function<void()> on_open);
  void safe_send(const std::string &host, const std::string& id, const std::string &msg);
public:
  remote_server(on_message_func on_message);
  void on_open_connection(server* s, websocketpp::connection_hdl conn);
  void on_close_connection(server* s, websocketpp::connection_hdl conn);
  void send(const std::string &id, const std::string &from, const std::string &msg);
};
}


#endif //DISTRIBUTED_WEBSOCKETS_REMOTE_SERVER_HPP
