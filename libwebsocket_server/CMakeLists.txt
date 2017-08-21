set(libwebsocket_server_SRC
    websocket_server.cpp
    websocket_server.hpp
    websocket_server_config.hpp
    )

add_library(libwebsocket_server STATIC
        ${libwebsocket_server_SRC}
        )

target_link_libraries(libwebsocket_server lib)

target_include_directories(libwebsocket_server PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})