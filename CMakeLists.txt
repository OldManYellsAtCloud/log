cmake_minimum_required(VERSION 3.16)

project(log LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(sdbus-c++ REQUIRED)

add_executable(log main.cpp
    log.h
    consolelog.h consolelog.cpp
    consolewriter.h consolewriter.cpp
    log.cpp
    filelog.h filelog.cpp
    socketlistener.h socketlistener.cpp
    loggerfactory.h loggerfactory.cpp
    requestprocessor.h requestprocessor.cpp
    enums.h)

include(GNUInstallDirs)
install(TARGETS log
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
