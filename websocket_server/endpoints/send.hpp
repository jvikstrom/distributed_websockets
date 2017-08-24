//
// Created by jovi on 2017-08-22.
//

#ifndef DISTRIBUTED_WEBSOCKETS_SEND_HPP
#define DISTRIBUTED_WEBSOCKETS_SEND_HPP

#include "base_endpoint.hpp"
namespace ws{
class send_endpoint : public base_endpoint{
  virtual void handle_message(server* s, websocketpp::connection_hdl hdl, json msg) override{
    if(msg["target"] == nullptr || msg["msg"] == nullptr){
      std::string ans = R"({"cmd": "error", "error": "Requires both 'target' and 'msg' to be set"})";
      s->send(hdl, ans, websocketpp::frame::opcode::text);
      return;
    }

    adapter->send(msg["target"].get<std::string>(), msg["msg"].get<std::string>(), hdl);
  }
};
}

#endif //DISTRIBUTED_WEBSOCKETS_SEND_HPP
