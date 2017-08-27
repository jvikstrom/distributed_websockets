//
// Created by jovi on 2017-08-21.
//

#include "websocket_server.hpp"

ws::websocket_server_config ws::websocket_server::default_config;
void ws::on_message(server *s, websocketpp::connection_hdl hdl, message_ptr msg) {

}
ws::websocket_server::websocket_server(websocket_server_config& config) : config(config){
  _server.set_access_channels(websocketpp::log::alevel::all);
  _server.clear_access_channels(websocketpp::log::alevel::frame_payload);

  // Initialize Asio
  _server.init_asio();

  // Register our message handler
  //_server.set_message_handler(std::bind(&config.on_message, &_server, ::_1, ::_2));
  _server.set_message_handler(bind(config.on_message, &_server,::_1,::_2));
  _server.set_open_handler(bind(config.on_open, &_server,::_1));
  _server.set_close_handler(bind(config.on_close, &_server,::_1));
}

void ws::websocket_server::start(){
  // Listen on port config.port
  _server.listen(config.port);

  // Start the server accept loop
  _server.start_accept();

  // Start the ASIO io_service run loop
  _server.run();
}

/*void ws::websocket_server::on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
  std::cout << "on message" << std::endl;
}*/