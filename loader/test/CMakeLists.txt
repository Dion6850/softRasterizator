# 添加loader测试可执行文件

cmake_minimum_required(VERSION 3.10)
project(loader_test)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/loader)
# 查找 loader 库
add_executable(loader_test main.cpp)
target_link_libraries(loader_test loader)
target_include_directories(loader_test PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../include
    ${CMAKE_CURRENT_LIST_DIR}/../../thirdPart/eigen-3.4.0
)
