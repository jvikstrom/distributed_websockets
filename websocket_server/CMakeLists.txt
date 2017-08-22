set(distributed_websockets_server_SRC
    main.cpp
    websocket_endpoints.cpp
    websocket_endpoints.hpp
    endpoints/base_endpoint.hpp
    endpoints/login.hpp
    endpoints/register.hpp
    endpoints/list_users.hpp)
add_executable(distributed_websockets_server ${distributed_websockets_server_SRC})

target_link_libraries(distributed_websockets_server libwebsocket_server lib libredis)

#${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}
install(TARGETS distributed_websockets_server DESTINATION bin)
