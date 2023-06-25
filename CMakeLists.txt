# define the project name
project(indi-KOBSpower C CXX)
cmake_minimum_required(VERSION 3.22)

include(GNUInstallDirs)

# add our cmake_modules folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "..." FORCE)
endif()

# find our required packages
find_package(INDI REQUIRED)
find_package(Nova REQUIRED)
find_package(ZLIB REQUIRED)
find_package(GSL REQUIRED)

# these will be used to set the version number in config.h and our driver's xml file
set(KOBSpower_VERSION_MAJOR 0)
set(KOBSpower_VERSION_MINOR 6)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/VERSION.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/VERSION
)

# do the replacement in the config.h
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# do the replacement in the driver's xml file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/indi_KOBSpower.xml.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/indi_KOBSpower.xml
)

# set our include directories to look for header files
include_directories( ${CMAKE_CURRENT_BINARY_DIR})
include_directories( ${CMAKE_CURRENT_SOURCE_DIR})
include_directories( ${INDI_INCLUDE_DIR})
include_directories( ${NOVA_INCLUDE_DIR})
include_directories( ${EV_INCLUDE_DIR})

include(CMakeCommon)

# tell cmake to build our executable
add_executable(
    indi_KOBSpower
    KOBSpower.cpp
)

# and link it to these libraries
target_link_libraries(
    indi_KOBSpower
    libpi-gpio.so
    ${INDI_LIBRARIES}
    ${NOVA_LIBRARIES}
    ${GSL_LIBRARIES}
)

# tell cmake where to install our executable
install(TARGETS indi_KOBSpower RUNTIME DESTINATION bin)

# and where to put the driver's xml file.
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/indi_KOBSpower.xml
    DESTINATION ${INDI_DATA_DIR}
    )

install (
    PROGRAMS
    ${CMAKE_CURRENT_SOURCE_DIR}/KOBScurrent.py
    DESTINATION ${INDI_DATA_DIR}/scripts
)


