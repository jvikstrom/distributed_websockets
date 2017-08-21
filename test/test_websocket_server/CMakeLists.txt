file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testlibwebsocket_server ${SRCS})

TARGET_LINK_LIBRARIES(testlibwebsocket_server
        libwebsocket_server
        gtest
        )

add_test(NAME testlibwebsocket_server
        COMMAND testlibwebsocket_server)
