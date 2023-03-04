cmake_minimum_required (VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)

project ("ZMQ_Server_RadioGigabit")

set(BUILD_SHARED OFF)
set(BUILD_TESTS OFF)

include(FetchContent)
FetchContent_Declare( 
	libzmq 
	GIT_REPOSITORY https://github.com/zeromq/libzmq.git 
	GIT_TAG        v4.3.4 
)
FetchContent_MakeAvailable(libzmq)
find_library (libzmq libzmq-static)

add_executable (ZMQ_Client_RadioGigabit "ZMQ_Client_RadioGigabit.cpp" "zmq.hpp")
target_link_libraries (ZMQ_Client_RadioGigabit libzmq-static)

add_executable (ZMQ_Server_RadioGigabit "ZMQ_Server_RadioGigabit.cpp" "zmq.hpp")
target_link_libraries (ZMQ_Server_RadioGigabit libzmq-static)
