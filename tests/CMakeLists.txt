
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_PATH ${PROJECT_SOURCE_DIR}/tests)

set(CMAKE_CXX_FLAGS "-std=c++17 -DEBUG")

include_directories(${INCLUDE_PATH})

set(src 
    ${SOURCE_PATH}/TimerTest.cpp)

set(EXECUTABLE_OUTPUT_PATH ${INSTALL_PATH})

add_executable(app_tests ${src})

target_link_libraries(app_tests
    z)

set_target_properties(app_tests PROPERTIES OUTPUT_NAME "unittest")