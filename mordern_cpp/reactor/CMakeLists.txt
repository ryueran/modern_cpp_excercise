# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(EchoServer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 定义变量以指定 libosmium 的路径，默认值为空
set(LIBOSMIUM_DIR "" CACHE PATH "Path to libosmium directory")

# 如果没有设置路径，则提示错误
if(NOT LIBOSMIUM_DIR)
    message(FATAL_ERROR "Please specify the path to libosmium using -DLIBOSMIUM_DIR=/path/to/libosmium")
endif()

# Include directories
include_directories(include ${LIBOSMIUM_DIR}/include)

# Find dependecy for curl
find_package(CURL REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Protobuf REQUIRED)

if(NOT TARGET Protobuf::Protobuf)
    add_library(Protobuf::Protobuf INTERFACE IMPORTED)
    set_target_properties(Protobuf::Protobuf PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Protobuf_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${Protobuf_LIBRARIES}")
endif()

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
add_executable(osm_parser utility/openstreetmap/src/parse_osm.cc)
# Link libraries (if needed, e.g., pthread for multithreading)
# target_link_libraries(EchoServer pthread)
target_link_libraries(osm_downloader CURL::libcurl)
target_link_libraries(osm_parser ${Boost_LIBRARIES} Threads::Threads ZLIB::ZLIB Protobuf::Protobuf)