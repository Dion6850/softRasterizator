file(GLOB_RECURSE LSR3D_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

# GLOB_RECURSE is used to find all header files in the include directory
file(GLOB_RECURSE LSR3D_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_library(Lsr3D STATIC ${LSR3D_HEADERS} ${LSR3D_SOURCES})
target_include_directories(Lsr3D PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)