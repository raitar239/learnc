cmake_minimum_required(VERSION 3.14)
project(Calculator)

# google test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0 
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# lib+app
add_library(calc_lib src/calculator.cpp)
add_executable(calculator_app src/main.cpp)
target_link_libraries(calculator_app calc_lib)

# tests
enable_testing()
add_executable(unit_tests tests/test_calculator.cpp)
target_link_libraries(unit_tests gtest_main calc_lib)

add_test(NAME AllTests COMMAND unit_tests)