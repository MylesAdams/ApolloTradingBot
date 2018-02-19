cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot C CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib)

include_directories(src)
include_directories(lib/rapidjson/include)
include_directories(lib/curlpp/include)
include_directories(lib/curl/include)

# If you make a new *.cpp file, place it here:
set(SOURCES
  ${SOURCES}
  #src/main.cpp
  src/Bot.cpp
  src/FourChan.cpp
  src/gdax_accnt.cpp
  src/exchange_manager.cpp
  src/binance_accnt.cpp
  src/api_key.cpp
  src/api_test.cpp
  #src/gui_test_driver.cpp
  src/kucoin_accnt.cpp
  )

# Put any header files you create here:
set(HEADERS
  ${HEADERS}
  src/Bot.h
  src/Comment.h
  src/FourChan.h
  src/kucoin_accnt.h
  src/NotFoundException.h
  src/gdax_accnt.h
  src/exchange_manager.h
  src/exchange_accnt.h
  src/coin.h
  src/binance_accnt.h
  src/api_key.h
  )

set(LIBRARIES 
  ${LIBRARIES}
  curlpp
  )

source_group("Source files" FILES ${SOURCES})
source_group("Header files" FILES ${HEADERS})

# It is unnecessary to link against the header files. The reason I added Headers is a hack to add the header
# files to the IDE project so they appear while working in an IDE.
add_executable(ApolloTradingBot ${SOURCES} ${HEADERS})

target_link_libraries(ApolloTradingBot ${LIBRARIES})


# On windows systems, this copies the newly created curlpp.dll into the same output folder as the executable
# after the project compiles.
if(WIN32)
  add_custom_command(TARGET ApolloTradingBot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Build/lib/curlpp/$<CONFIGURATION>"
        $<TARGET_FILE_DIR:ApolloTradingBot>)
endif()