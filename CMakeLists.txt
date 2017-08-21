cmake_minimum_required(VERSION 3.8)
project(distributed_websockets)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set(RUN_TESTS ON)
if(${RUN_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()

add_subdirectory(websocket_server)
add_subdirectory(libwebsocket_server)
add_subdirectory(lib)