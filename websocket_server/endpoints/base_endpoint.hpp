//
// Created by jovi on 2017-08-21.
//

#ifndef DISTRIBUTED_WEBSOCKETS_ENDPOINT_HPP_HPP
#define DISTRIBUTED_WEBSOCKETS_ENDPOINT_HPP_HPP

#include <websocket_server.hpp>
#include <json/json.hpp>
#include <redis_adapter.hpp>

using json = nlohmann::json;

namespace ws{
class base_endpoint{
protected:
  redis_adapter *adapter;

public:
  virtual void handle_message(server* s, websocketpp::connection_hdl hdl, json msg) = 0;
  void set_adapter(redis_adapter* adapter){
    this->adapter = adapter;
  }
};
}
#endif //DISTRIBUTED_WEBSOCKETS_ENDPOINT_HPP_HPP
