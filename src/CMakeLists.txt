cmake_minimum_required(VERSION 3.20)
project(compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Fetching Nlohmann/json
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(lang_core
  base/Lexer.cpp
)
target_link_libraries(lang_core PUBLIC nlohmann_json::nlohmann_json)

add_executable(compiler main.cpp)
target_link_libraries(compiler PRIVATE lang_core)
