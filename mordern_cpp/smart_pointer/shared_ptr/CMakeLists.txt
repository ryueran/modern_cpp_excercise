cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 --coverage")

find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main_shared_ptr main_shared_ptr.cpp)

# Add test source file (Widget.hh is included in test_widget.cpp)
add_executable(WidgetTest test_main_shared_ptr.cpp)

# Link GTest and pthread libraries
target_link_libraries(WidgetTest PRIVATE GTest::gtest GTest::gtest_main pthread)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Add coverage flags for GCC or Clang
    target_compile_options(WidgetTest PRIVATE --coverage)
    target_link_options(WidgetTest PRIVATE --coverage)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(--coverage)
    add_link_options(--coverage)
endif()

# Enable testing
enable_testing()
add_test(NAME WidgetTest COMMAND WidgetTest)
gtest_discover_tests(WidgetTest)

# Run tests automatically after building
add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS WidgetTest
)