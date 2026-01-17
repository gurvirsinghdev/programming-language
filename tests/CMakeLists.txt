cmake_minimum_required(VERSION 3.20)
project(compiler_tests LANGUAGES CXX)


add_executable(compiler_tests main.cpp)
target_link_libraries(compiler_tests PRIVATE lang_core)

enable_testing()
add_test(NAME unit_tests COMMAND compiler_tests)