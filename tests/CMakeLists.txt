cmake_minimum_required(VERSION 3.20)
project(compiler_tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(compiler_tests main.cpp)
target_link_libraries(compiler_tests 
  PRIVATE 
  # lang_core
  GTest::gtest
  GTest::gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(compiler_tests)