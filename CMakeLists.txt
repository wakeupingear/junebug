#https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake

set(LIBNAME junebug)

cmake_minimum_required(VERSION 3.9)
project(${LIBNAME} VERSION 0.1.0 DESCRIPTION "A lightweight C++ game engine")
include(GNUInstallDirs)

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

add_library(${LIBNAME} SHARED
    src/Junebug.cpp
    src/dataLoading.cpp
    src/Math.cpp
    src/inputs.cpp
)

set_target_properties(${LIBNAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/${LIBNAME}.h)
configure_file(${LIBNAME}.pc.in ${LIBNAME}.pc @ONLY)
target_include_directories(${LIBNAME} PRIVATE include)
target_include_directories(${LIBNAME} PRIVATE src)
target_compile_features(${LIBNAME} PRIVATE cxx_std_17)

include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2) 
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(
    ${LIBNAME} 
    ${SDL2_LIBRARIES} 
    ${SDL2IMAGE_LIBRARIES}
    ${SDL2MIXER_LIBRARIES}
)

install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/${LIBNAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)