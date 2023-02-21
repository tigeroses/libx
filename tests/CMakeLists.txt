
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include $ENV{CPLUS_INCLUDE_PATH})
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/tests)

# set(CMAKE_CXX_FLAGS "-std=c++17 -g -O0 --coverage")
set(CMAKE_CXX_FLAGS "-std=c++17 -O2")

include_directories(${INCLUDE_PATH})

aux_source_directory(. src)

set(EXECUTABLE_OUTPUT_PATH ${INSTALL_PATH})

add_executable(app_tests ${src})

target_link_libraries(app_tests
    pthread
    z
)

set_target_properties(app_tests PROPERTIES OUTPUT_NAME "unittest")