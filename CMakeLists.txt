cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

add_executable(ApolloTradingBot src/main.cpp)
