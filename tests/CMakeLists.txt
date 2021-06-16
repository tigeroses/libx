
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/tests)

set(CMAKE_CXX_FLAGS "-std=c++17 -DEBUG")

include_directories(${INCLUDE_PATH})

aux_source_directory(. src)
# set(src DIR_SRCS)

set(EXECUTABLE_OUTPUT_PATH ${INSTALL_PATH})

add_executable(app_tests ${src})

target_link_libraries(app_tests
    pthread    
)

set_target_properties(app_tests PROPERTIES OUTPUT_NAME "unittest")