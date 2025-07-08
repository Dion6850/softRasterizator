
# 添加3D模型导入模块
set(LODER_SRC
    src/ModelLoader.cpp
)

set(LODER_INCLUDE
    include/ModelLoader.h
)


add_library(loader ${LODER_SRC} ${LODER_INCLUDE})
target_include_directories(loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

