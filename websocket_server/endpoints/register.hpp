//
// Created by jovi on 2017-08-21.
//

#ifndef DISTRIBUTED_WEBSOCKETS_REGISTER_HPP
#define DISTRIBUTED_WEBSOCKETS_REGISTER_HPP

#include "base_endpoint.hpp"
namespace ws{
class login_endpoint : public base_endpoint{
  virtual void handle_message(server* s, websocketpp::connection_hdl hdl, json msg) override{

  }
};
}

#endif //DISTRIBUTED_WEBSOCKETS_REGISTER_HPP
