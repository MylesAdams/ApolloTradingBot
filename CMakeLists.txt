cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot C CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

file(GLOB rapidjson_lib "lib/rapidjson/*.h" "lib/rapidjson/error/*.h" "lib/rapidjson/internal/*.h" "lib/rapidjson/msinttypes/*.h")

include_directories(lib/rapidjson)
include_directories(lib/curlpp/include)

add_executable(ApolloTradingBot src/main.cpp)

target_link_libraries(ApolloTradingBot curlpp)
