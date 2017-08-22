#gtest_main
set(CPP_REDIS_INCLUDE_DIR "/usr/local/include/")
set(CPP_REDIS_LIBRARIES "/usr/local/lib/libcpp_redis.a")
set(TACOPIE_INCLUDE_DIR "/usr/local/include/")
set(TACOPIE_LIBRARIES "/usr/local/lib/libtacopie.a")

set(lib_SRC
    json/json.hpp
    websocketpp/server.hpp
    linker_file.cpp)

FIND_PACKAGE( Boost COMPONENTS system REQUIRED )
FIND_PACKAGE(Threads)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ${CPP_REDIS_INCLUDE_DIR} ${TACOPIE_INCLUDE_DIR})

add_library(lib STATIC
    ${lib_SRC}
    )

message(${Boost_INCLUDE_DIR})
target_link_libraries(lib ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${CPP_REDIS_LIBRARIES} ${TACOPIE_LIBRARIES})

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#add_subdirectory(websocket)
