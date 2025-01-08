# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(EchoServer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Find dependecy for curl
find_package(CURL REQUIRED)

# Source files
set(SRC_FILES
    src/main_server.cc        # Entry point
    src/Acceptor.cc
    src/Epoller.cc
    src/Handler.cc
    src/Reactor.cc
    src/Server.cc
)

# Create the executable
add_executable(EchoServer ${SRC_FILES})
add_executable(EchoClient src/main_client.cc)
add_executable(server_simple src/main_server_simple.cc)
add_executable(osm_downloader utility/openstreetmap/src/download_osm.cc)
# Link libraries (if needed, e.g., pthread for multithreading)
# target_link_libraries(EchoServer pthread)
target_link_libraries(osm_downloader CURL::libcurl)