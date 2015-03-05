cmake_minimum_required(VERSION 2.8.11)
project(obdii-query)
find_package(Qt5Core REQUIRED)

include_directories(src/jsoncpp)
add_library(jsoncpp STATIC src/jsoncpp/jsoncpp.cpp)

add_executable(carcomm src/main.cpp)
target_link_libraries(carcomm Qt5::Core)
target_link_libraries(carcomm jsoncpp)
