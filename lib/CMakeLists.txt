#gtest_main
set(lib_SRC
    json/json.hpp
    websocketpp/server.hpp
    linker_file.cpp)

FIND_PACKAGE( Boost COMPONENTS system REQUIRED )
FIND_PACKAGE(Threads)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR})

add_library(lib STATIC
    ${lib_SRC}
    )

target_link_libraries(lib ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#add_subdirectory(websocket)
