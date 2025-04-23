cmake_minimum_required(VERSION 3.11)
project(pg2_1)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)

FetchContent_MakeAvailable(json)

FILE(GLOB_RECURSE sources src/*.cpp)
FILE(GLOB_RECURSE headers src/*.h)

add_executable(pg2_1 ${sources} ${headers})
target_link_libraries(pg2_1 PRIVATE nlohmann_json::nlohmann_json)
