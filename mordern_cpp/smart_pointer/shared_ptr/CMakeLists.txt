set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main_shared_ptr main_shared_ptr.cpp)


# Add test source file (Widget.hh is included in test_widget.cpp)
add_executable(WidgetTest test_main_shared_ptr.cpp)

# Link GTest and pthread libraries
target_link_libraries(WidgetTest PRIVATE GTest::gtest GTest::gtest_main pthread)

# Enable testing
enable_testing()
add_test(NAME WidgetTest COMMAND WidgetTest)

# Run tests automatically after building
add_custom_target(run_tests ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS WidgetTest
)