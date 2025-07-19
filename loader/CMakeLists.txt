set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LODER_SRC
    src/ModelLoader.cpp
)

set(LODER_INCLUDE
    include/ModelLoader.h
)

add_library(loader ${LODER_SRC} ${LODER_INCLUDE})
target_include_directories(loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

