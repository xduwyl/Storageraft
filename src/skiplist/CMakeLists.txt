# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(SkipListProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Boost  REQUIRED COMPONENTS serialization)

# 查找 pthread 库
find_package(Threads REQUIRED)

# 添加跳表库（skiplist.cpp 和 skiplist.h）
add_library(SkipList skiplist.cpp skiplist.h)

# 添加测试可执行文件
add_executable(SkipListTest test.cpp)
target_link_libraries(SkipListTest SkipList Threads::Threads Boost::boost Boost::serialization) # 链接跳表库和 pthread

# 添加压力测试可执行文件
add_executable(StressTest stress-test/stress_test.cpp)
target_link_libraries(StressTest SkipList Threads::Threads)  # 链接跳表库和 pthread

# 可选：启用调试信息
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(SkipListTest PRIVATE -g)
    target_compile_options(StressTest PRIVATE -g)
endif()
