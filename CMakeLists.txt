project(ApolloTradingBot C CXX)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_DEBUG_RUNTIME OFF)
set(Boost_USE_DEBUG_PYTHON OFF)
set(Boost_USE_STLPORT OFF)
find_package(Boost REQUIRED COMPONENTS
    atomic
    chrono
    date_time
    exception
    filesystem
    program_options
    random
    regex
    system
    serialization
    thread
    )

IF (!Boost_FOUND)
    MESSAGE("*** ERROR *** Boost package not found")
    RETURN()
ENDIF ()   

include_directories(${Boost_INCLUDE_DIRS})
MESSAGE("Boost_INCLUDE_DIRS:\t" ${Boost_INCLUDE_DIRS})


find_package(cpprestsdk REQUIRED)
find_package(OpenSSL REQUIRED)

set(MY_INCLUDE_DIRS
${MY_INCLUDE_DIRS}
${OPENSLL_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
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

set(LIBRARIES 
  ${LIBRARIES}
  curlpp
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
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