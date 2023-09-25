# Add test cpp file
add_executable(runUnitTests test_main.cpp)

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
