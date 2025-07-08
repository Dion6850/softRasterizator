cmake_minimum_required(VERSION 3.20)
project(softResterizator)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCE_FILES src/utils/*.cpp
        src/core/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)
find_package(GLUT REQUIRED)
if(GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES})
    message("GLUT include found at: ${GLUT_INCLUDE_DIR}")
    message("GLUT library found at: ${GLUT_LIBRARIES}")
else(GLUT_FOUND)
    message(FATAL_ERROR "GLUT library not found")
endif(GLUT_FOUND)
if(MSVC)
    add_compile_options(/utf-8)
    # This tells MSVC to interpret source files as UTF-8
endif()

include_directories(thirdPart/eigen-3.4.0)