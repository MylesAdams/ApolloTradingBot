cmake_minimum_required(VERSION 3.0)
project(ApolloTradingBot C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if (APPLE)
  set(OPENSSL_ROOT_DIR /usr/local/opt/openssl/)
  set(OPT_CPPFLAGS "-stdlib=libc++ -I/usr/local/opt/openssl/include")
  set(OPT_LDFLAGS "-L/usr/local/opt/openssl/lib -lcpprest -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_CPPFLAGS} -v -g -O3 -std=c++11 -fno-rtti")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OPT_LDFLAGS}")
#  set(Qt5_DIR "/usr/local/Cellar/qt/5.10.1")
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

FIND_PACKAGE(Boost 1.66 COMPONENTS program_options REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
# FIND_PACKAGE(Qt5Widgets REQUIRED)
# FIND_PACKAGE(Qt5PrintSupport REQUIRED)
find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)

find_package(cpprestsdk REQUIRED)

include_directories(src)
include_directories(lib/rapidjson/include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )


# If you make a new *.cpp file, place it here:
set(SOURCES
  ${SOURCES}
#  src/gdax_accnt.cpp
#  src/exchange_manager.cpp
#  src/binance_accnt.cpp
#  src/api_key.cpp
  # src/api_test.cpp
  #src/gui_test_driver.cpp
#  src/kucoin_accnt.cpp
  src/qcustomplot.cpp
  src/main.cpp
  src/mainwindow.cpp
  src/Bot.cpp
  # src/FourChan.cpp
  src/Twitter.cpp
  src/watson.cpp
  )

# Put any header files you create here:
set(HEADERS
  ${HEADERS}
  #  src/Comment.h
  #  src/kucoin_accnt.h
  #  src/NotFoundException.h
  #  src/gdax_accnt.h
  #  src/exchange_manager.h
  #  src/exchange_accnt.h
  #  src/coin.h
  #  src/binance_accnt.h
  #  src/api_key.h
    src/qcustomplot.h
    src/mainwindow.h
    src/mainwindow.ui
    src/Bot.h
    src/Comment.h
    # src/FourChan.h
    src/Twitter.h
    src/watson.h
  )

set(LIBRARIES
  ${LIBRARIES}
  cpprestsdk::cpprest
  cpprestsdk::cpprestsdk_boost_internal
  cpprestsdk::cpprestsdk_zlib_internal
  cpprestsdk::cpprestsdk_openssl_internal
  Qt5::Widgets
  Qt5::PrintSupport
  )

source_group("Source files" FILES ${SOURCES})
source_group("Header files" FILES ${HEADERS})

# It is unnecessary to link against the header files. The reason I added Headers is a hack to add the header
# files to the IDE project so they appear while working in an IDE.
add_executable(ApolloTradingBot ${SOURCES} ${HEADERS})

target_link_libraries(ApolloTradingBot PRIVATE ${LIBRARIES} ${Boost_LIBRARIES} )
