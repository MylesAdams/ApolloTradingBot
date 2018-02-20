cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot C CXX)

if (APPLE)
  set(OPENSSL_ROOT_DIR /usr/local/opt/openssl/)
  set(OPT_CPPFLAGS "-stdlib=libc++ -I/usr/local/opt/openssl/include")
  set(OPT_LDFLAGS "-L/usr/local/opt/openssl/lib -lcpprest -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_CPPFLAGS} -v -g -O3 -std=c++11 -fno-rtti")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OPT_LDFLAGS}")
endif()

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPLIER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPLIER_SUPPORTS_CXX0X)
if(COMPLIER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

FIND_PACKAGE( Boost 1.66 COMPONENTS program_options REQUIRED )
FIND_PACKAGE(OpenSSL REQUIRED)

include_directories(src)
include_directories(lib/rapidjson/include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# If you make a new *.cpp file, place it here:
set(SOURCES
  ${SOURCES}
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
  src/Comment.h
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
  )

source_group("Source files" FILES ${SOURCES})
source_group("Header files" FILES ${HEADERS})

# It is unnecessary to link against the header files. The reason I added Headers is a hack to add the header
# files to the IDE project so they appear while working in an IDE.
add_executable(ApolloTradingBot ${SOURCES} ${HEADERS})

target_link_libraries(ApolloTradingBot PRIVATE ${LIBRARIES} ${Boost_LIBRARIES})


# On windows systems, this copies the newly created curlpp.dll into the same output folder as the executable
# after the project compiles.
if(WIN32)
  add_custom_command(TARGET ApolloTradingBot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Build/lib/curlpp/$<CONFIGURATION>"
        $<TARGET_FILE_DIR:ApolloTradingBot>)
endif()
