cmake_minimum_required(VERSION 2.8.11)
project(obdii-query)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Bluetooth REQUIRED)
find_package(Qt5Test REQUIRED)

add_executable(carcomm src/main.cpp src/serialcomms.cpp src/CarCoreApp.cpp)
set_property(TARGET carcomm PROPERTY CXX_STANDARD 11)
target_link_libraries(carcomm Qt5::Core)
target_link_libraries(carcomm Qt5::SerialPort)
target_link_libraries(carcomm Qt5::Bluetooth)
target_link_libraries(carcomm Qt5::Test)
