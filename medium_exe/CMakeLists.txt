cmake_minimum_required(VERSION 3.18)

project(medium_unique_ptr)

set(CMAKE_CXX_STANDARD 23)

add_executable(Reactor Src/main.cpp)
add_executable(Serialize Src/main1.cpp)
add_executable(Shared_ptr Src/main_shared_ptr.cpp)

include_directories(Inc)
