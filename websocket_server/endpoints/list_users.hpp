//
// Created by jovi on 2017-08-22.
//

#ifndef DISTRIBUTED_WEBSOCKETS_LIST_USERS_HPP
#define DISTRIBUTED_WEBSOCKETS_LIST_USERS_HPP

#include "base_endpoint.hpp"
namespace ws{
class list_users_endpoint : public base_endpoint{
  virtual void handle_message(server* s, websocketpp::connection_hdl hdl, json msg) override{
    std::vector<std::string> users = adapter->list_connections(hdl);
    json j(users);
    json j_payload;
    j_payload["cmd"] = "list";
    j_payload["users"] = j;
    std::string payload = j_payload.dump();
    s->send(hdl, payload, websocketpp::frame::opcode::text);
  }
};
}
#endif //DISTRIBUTED_WEBSOCKETS_LIST_USERS_HPP
