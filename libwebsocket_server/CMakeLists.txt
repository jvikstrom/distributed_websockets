set(libwebsocket_server_SRC
        )

add_library(libwebsocket_server STATIC
        ${libwebsocket_server_SRC}
        )

target_link_libraries(libwebsocket_server)

target_include_directories(libwebsocket_server PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})