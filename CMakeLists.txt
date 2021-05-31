
cmake_minimum_required(VERSION 3.5)

# set(CMAKE_SKIP_BUILD_RPATH FALSE)
# set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TURE)

project(libx)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(tests)

message(STATUS "Compile Done")