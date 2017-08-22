//
// Created by jovi on 2017-08-21.
//

#include "websocket_endpoints.hpp"

void ws::websocket_endpoints::handle_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg){
  json js;
  adapter.set_server(s);
  try{
    js = json::parse(msg->get_payload());
  } catch(nlohmann::detail::parse_error &e){
    std::string ans_payload = R"({"error": "Could not parse json"})";
    s->send(hdl, ans_payload, msg->get_opcode());
    return;
  }

  std::string cmd;
  if(js["cmd"] == nullptr){
    cmd = "";
  }else{
    cmd = js["cmd"].get<std::string>();
  }

  auto it = endpoints.find(cmd);
  if(it == endpoints.end()){
    std::string ans_payload = R"({"error": "Could find cmd"})";
    s->send(hdl, ans_payload, msg->get_opcode());
    return;
  }

  it->second->handle_message(s, hdl, js);
}

void ws::websocket_endpoints::add_endpoint(std::string cmd, std::unique_ptr<ws::base_endpoint> endpoint){
  endpoint->set_adapter(&this->adapter);
  endpoints[cmd] = std::move(endpoint);
}