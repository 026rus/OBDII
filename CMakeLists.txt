cmake_minimum_required(VERSION 3.1.0)
project(obdii-query)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5Core REQUIRED)
find_package(Qt5SerialPort REQUIRED)
find_package(Qt5Bluetooth REQUIRED)
find_package(Qt5Test REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Network REQUIRED)

INCLUDE_DIRECTORIES(src)
add_executable(carcomm src/main.cpp
    src/PortReaderWriter.cpp
    src/CarCoreApp.cpp
    src/mainwindow.cpp
    src/qcustomplot.cpp
    src/ParseJson.cpp
    src/qcpdocumentobject.cpp
    src/obd2client.cpp
    src/DataOut.cpp
)

set_property(TARGET carcomm PROPERTY CXX_STANDARD 11)

target_link_libraries(carcomm Qt5::Core
    Qt5::SerialPort
    Qt5::Bluetooth
    Qt5::Test
    Qt5::Widgets
    Qt5::PrintSupport
    Qt5::Network
)

add_custom_command(TARGET carcomm PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/data
	$<TARGET_FILE_DIR:carcomm>/data
    )

install(TARGETS carcomm RUNTIME DESTINATION bin)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/data/bcodes.json
    ${CMAKE_CURRENT_SOURCE_DIR}/data/ccodes.json
    ${CMAKE_CURRENT_SOURCE_DIR}/data/pcodes.json
    ${CMAKE_CURRENT_SOURCE_DIR}/data/ucodes.json
  DESTINATION data
)
