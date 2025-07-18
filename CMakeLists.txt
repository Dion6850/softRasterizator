cmake_minimum_required(VERSION 3.20)
project(softResterizator)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCE_FILES src/utils/*.cpp
        src/core/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)

# test
add_executable(test_load_image ${SOURCE_FILES} test/load_image.cpp)

find_package(GLUT REQUIRED)
if(GLUT_FOUND)
    include_directories(${GLUT_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES})
    target_link_libraries(test_load_image ${GLUT_LIBRARIES})
    message("GLUT include found at: ${GLUT_INCLUDE_DIR}")
    message("GLUT library found at: ${GLUT_LIBRARIES}")
else(GLUT_FOUND)
    message(FATAL_ERROR "GLUT library not found")
endif(GLUT_FOUND)
if(MSVC)
    add_compile_options(/utf-8)
    # This tells MSVC to interpret source files as UTF-8
endif()

include_directories(thirdPart)
# 使用stb的模块需要提前定义
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(test_load_image PRIVATE STB_IMAGE_IMPLEMENTATION)
