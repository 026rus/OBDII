cmake_minimum_required(VERSION 3.0.2)
project(obdii-query)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Bluetooth REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

INCLUDE_DIRECTORIES(src)
add_executable(carcomm src/main.cpp
    src/PortReaderWriter.cpp
    src/CarCoreApp.cpp
    src/mainwindow.cpp
    src/qcustomplot.cpp)

set_property(TARGET carcomm PROPERTY CXX_STANDARD 11)

target_link_libraries(carcomm Qt5::Core
    Qt5::SerialPort
    Qt5::Bluetooth
    Qt5::Test
    Qt5::Widgets
    Qt5::PrintSupport)
