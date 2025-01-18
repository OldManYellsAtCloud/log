cmake_minimum_required(VERSION 3.16)

project(log LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(log src/main.cpp
    include/log.h
    include/consolelog.h src/consolelog.cpp
    include/consolewriter.h src/consolewriter.cpp
    src/log.cpp
    include/filelog.h src/filelog.cpp
    include/socketlistener.h src/socketlistener.cpp
    include/loggerfactory.h src/loggerfactory.cpp
    include/requestprocessor.h src/requestprocessor.cpp
    include/enums.h)

target_include_directories(log
    PRIVATE include
)

set_target_properties(log PROPERTIES VERSION "1.0")
set(LOG_PUBLIC_HEADERS "include/log.h" "include/enums.h")
set_target_properties(log PROPERTIES PUBLIC_HEADER "${LOG_PUBLIC_HEADERS}")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "logConfig.cmake"
    VERSION 1.0
    COMPATIBILITY AnyNewerVersion)


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/logConfig.cmake"
        DESTINATION lib/cmake/log)

include(GNUInstallDirs)
install(TARGETS log
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/log
)
