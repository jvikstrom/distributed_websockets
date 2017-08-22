#ifndef LOGIN_ENDPOINT
#define LOGIN_ENDPOINT

#include "base_endpoint.hpp"

namespace ws{
class login_endpoint : public base_endpoint{
  virtual void handle_message(server* s, websocketpp::connection_hdl hdl, json msg) override{
    if(msg["username"] == nullptr || msg["password"] == nullptr){
      std::string ans = R"({"error": "Requires both 'username' and 'password' to be set"})";
      s->send(hdl, ans, websocketpp::frame::opcode::text);
      return;
    }

    adapter->login(msg["username"].get<std::string>(), msg["password"].get<std::string>(), hdl);
  }
};
}

#endif