set(distributed_websockets_server_SRC main.cpp )
add_executable(distributed_websockets_server ${distributed_websockets_server_SRC})

target_link_libraries(distributed_websockets_server libwebsocket_server)

#${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT}
install(TARGETS distributed_websockets_server DESTINATION bin)
