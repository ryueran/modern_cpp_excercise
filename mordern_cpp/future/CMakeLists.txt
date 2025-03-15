find_package(Threads REQUIRED)
add_executable(EventTrigger EventTrigger.cpp)
target_link_libraries(EventTrigger pthread)
