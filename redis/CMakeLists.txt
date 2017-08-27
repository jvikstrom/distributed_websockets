set(libredis_SRC
    redis_adapter.cpp
    redis_adapter.hpp
    remote_server.cpp
    remote_server.hpp
    )

add_library(libredis STATIC
    ${libredis_SRC}
    )

target_link_libraries(libredis lib libwebsocket_server)

target_include_directories(libredis PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})