cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot C CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

# lib/rapidjson/*.h lib/curlpp/include/curlpp/*.hpp
# set(SOURCES src/main.cpp lib/rapidjson/allocators.h)

file(GLOB rapidjson_lib "lib/rapidjson/*.h" "lib/rapidjson/error/*.h" "lib/rapidjson/internal/*.h" "lib/rapidjson/msinttypes/*.h")

# add_library(rjson ${rapidjson_lib})
# set_target_properties(rjson PROPERTIES LINKER_LANGUAGE C)
include_directories(lib/rapidjson)

add_executable(ApolloTradingBot src/main.cpp)

# target_link_libraries(ApolloTradingBot rjson)
