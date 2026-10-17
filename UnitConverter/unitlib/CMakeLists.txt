# 把 src/unitlib.cpp 编成静态库，目标名 unitlib
add_library(unitlib STATIC src/unitlib.cpp)

# PUBLIC：谁链接 unitlib，谁就自动获得这个头文件搜索路径
target_include_directories(unitlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
