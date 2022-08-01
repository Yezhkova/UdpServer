cmake_minimum_required(VERSION 3.14)

project(UDPserver LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
include_directories(include)
set(SOURCES     main.cpp
                udpserver.cpp
                udpserver.h
                clientdata.h
                serialization.h
    )


add_executable(UDPserver ${SOURCES})


#target_link_libraries(main Qt${QT_VERSION_MAJOR}::Core)
