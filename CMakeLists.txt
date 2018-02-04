cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot C CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

file(GLOB rapidjson_lib "lib/rapidjson/*.h" "lib/rapidjson/error/*.h" "lib/rapidjson/internal/*.h" "lib/rapidjson/msinttypes/*.h")

include_directories(src)
include_directories(lib/rapidjson)
include_directories(lib/curlpp/include)
include_directories(lib/curl/include)

# If you make a new *.cpp file, place it here:
set(SOURCES
  ${SOURCES}
  src/main.cpp
  src/Bot.cpp
  )

# Put any header files you create here:
set(HEADERS
  ${HEADERS}
  src/Bot.h
  )

source_group("Source files" FILES ${SOURCES})
source_group("Header files" FILES ${HEADERS})

# It is unnecessary to link against the header files. The reason I added Headers is a hack to add the header files to the IDE project so they appear while working in an IDE.
add_executable(ApolloTradingBot ${SOURCES} ${HEADERS})

target_link_libraries(ApolloTradingBot curlpp)

if(WIN32)
  add_custom_command(TARGET ApolloTradingBot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Build/lib/curlpp/$<CONFIGURATION>"
        $<TARGET_FILE_DIR:ApolloTradingBot>)
endif()