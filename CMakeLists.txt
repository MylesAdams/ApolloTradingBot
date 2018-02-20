project(ApolloTradingBot C CXX)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

set(OPENSSL_ROOT_DIR "C:/vcpkg/packages/openssl_x86-windows")


FIND_PACKAGE(Boost REQUIRED )
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)

find_path(CPPRESTSDK_INCLUDE_DIRS "cpprest/http_client.h")
message("cpprest include:\t" ${CPPRESTSDK_INCLUDE_DIRS})
set(MY_INCLUDE_DIRS
${MY_INCLUDE_DIRS}
${CPPRESTSDK_INCLUDE_DIRS}
src/
lib/rapidjson/include
lib/curlpp/include  
lib/curl/include
)

add_subdirectory(lib)

# If you make a new *.cpp file, place it here:
set(SOURCES
  ${SOURCES}
  #src/main.cpp
  src/Bot.cpp
  src/FourChan.cpp
  src/api_test.cpp
  )

# Put any header files you create here:
set(HEADERS
  ${HEADERS}
  src/Bot.h
  src/Comment.h
  src/FourChan.h
  )

find_library(CPPRESTSDK_LIBRARIES "cpprest_2_10.lib")
message("cpprestsdk_libraries:\t" ${CPPRESTSDK_LIBRARIES})
set(LIBRARIES 
  ${LIBRARIES}
  curlpp
  ${CPPRESTSDK_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES}
  )

include_directories(${MY_INCLUDE_DIRS})

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