cmake_minimum_required(VERSION 2.8.11)
project(obdii-query)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Bluetooth REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets)

add_executable(carcomm src/main.cpp src/PortReaderWriter.cpp src/CarCoreApp.cpp GUI2/mainwindow.cpp GUI2/mainwindow.h GUI/ui_mainwindow.h GUI2/mainwindow.ui)
set_property(TARGET carcomm PROPERTY CXX_STANDARD 11)
target_link_libraries(carcomm Qt5::Core)
target_link_libraries(carcomm Qt5::SerialPort)
target_link_libraries(carcomm Qt5::Bluetooth)
target_link_libraries(carcomm Qt5::Test)
target_link_libraries(carcomm Qt5::Widgets)
