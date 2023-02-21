
cmake_minimum_required(VERSION 3.5)

project(libx)

set(CMAKE_BUILD_TYPE Release)

add_subdirectory(tests)

set(PREFIX ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY include/libx DESTINATION ${PREFIX})

message(STATUS "Compile Done")