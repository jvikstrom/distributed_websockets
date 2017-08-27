//
// Created by jovi on 2017-08-26.
//

#include "remote_server.hpp"

ws::remote_server::remote_server(on_message_func on_message) {
  internal_server_thread = std::thread();
  websocket_server_config config(port, on_message, std::bind(&ws::remote_server::on_open_connection, this, ::_1, ::_2), std::bind(&ws::remote_server::on_close_connection, this, ::_1, ::_2));
  s = std::make_unique<ws::websocket_server>(config);
  client.connect(redis_host);
  internal_server_thread = std::thread(std::bind(&ws::websocket_server::start, s.get()));
}

void ws::remote_server::on_open_connection(server *s, websocketpp::connection_hdl conn) {
  std::string host = s->get_con_from_hdl(conn)->get_host();
}

void ws::remote_server::on_close_connection(server *s, websocketpp::connection_hdl conn) {
  std::string host = s->get_con_from_hdl(conn)->get_host();
}

void ws::remote_server::send(const std::string &id, const std::string &from, const std::string &msg) {
  client.get(id, [this, id, msg](cpp_redis::reply& reply){
    if(reply.is_null()){
      return;
    }

    safe_send(reply.as_string(), id, msg);
  });
}

void ws::remote_server::safe_send(const std::string &host, const std::string& id, const std::string &msg){
  auto send = [this, host, msg](){
    client_mutex.lock_shared();
    auto it = open_clients.find(host);
    if(it == open_clients.end()){
      return;
    }

    it->second->send(websocketpp::connection_hdl(), msg, websocketpp::frame::opcode::text);
    //client.first->send(client.second, msg, websocketpp::frame::opcode::text);
  };


  auto send_inner = [this, host, send, msg](){
    client_mutex.lock_shared();
    auto it = open_clients.find(host);
    if(it == open_clients.end()){
      client_mutex.unlock_shared();
      open_connection(host, send);
      return;
    }

    client_mutex.unlock_shared();
    send();
  };

  send_inner();
}

void ws::remote_server::open_connection(const std::string& host, std::function<void()> on_open = std::function<void()>()){
//These clients are used as simplex streams. Messages are only going from client -> server. Never server -> client
  websocket_client client;
  client->init_asio();
  client->set_open_handler([host, on_open](websocketpp::connection_hdl conn){
    std::cout << "Opened internal client to: " << host << std::endl;
    on_open();
  });
  client->set_close_handler([host](websocketpp::connection_hdl conn){
    //TODO: Remove from the map
    std::cout << "Internal client to: " << host << " was closed" << std::endl;
  });

  websocketpp::lib::error_code ec;
  websocketpp::client<websocketpp::config::asio_client>::connection_ptr con = client->get_connection(host + ":" + std::to_string(port), ec);
  client->connect(con);

  client_mutex.lock();
  open_client_threads[host] = std::thread(std::bind(&websocketpp::client<websocketpp::config::asio_client>::run, client));
  open_clients[host] = client;
  client_mutex.unlock();
}