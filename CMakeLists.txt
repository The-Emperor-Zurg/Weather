cmake_minimum_required(VERSION 3.22)
project(Weather)

set(CMAKE_CXX_STANDARD 17)

add_executable(Weather main.cpp conio.h)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba)
FetchContent_MakeAvailable(cpr)

target_link_libraries(Weather PRIVATE cpr::cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(Weather PRIVATE nlohmann_json::nlohmann_json)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Config.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
