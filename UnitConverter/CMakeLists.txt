cmake_minimum_required(VERSION 3.16)      # 最低版本
project(UnitConverter LANGUAGES CXX)      # 项目名 + 只启用 C++

add_subdirectory(unitlib)                 # 把 unitlib 子目录纳入构建

add_executable(converter src/main.cpp)    # 生成可执行文件 converter

# 链接 unitlib 目标。头文件路径不用写：unitlib 用 PUBLIC 暴露出去了，会自动传播
target_link_libraries(converter PRIVATE unitlib)
