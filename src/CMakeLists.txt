# cmake 的最小版本要求
cmake_minimum_required(VERSION 3.12)
# 这个CMakeLists管理的工程名称
project(cmakeTest)
set(CMAKE_CXX_STANDARD 14)
set(GOOGLETEST_VERSION 1.13.0)

#Add the google test subdirectory
add_subdirectory(./googletest)

#include googletest/include dir
include_directories(./googletest/include)

#生成可执行程序 语法:add_executable(可执行程序名 要编译的cpp)
# add_executable(cmakeTest main.cpp engine.cpp paramParser.cpp output.cpp api.cpp test/unitTest.cpp test/randomTest.cpp)
add_executable(cmakeTest main.cpp engine.cpp paramParser.cpp api.cpp test/unitTest.cpp test/randomTest.cpp)

include_directories(./include)
#add_library(cmakeTest SHARED main.cpp engine.cpp paramParser.cpp output.cpp api.cpp test/unitTest.cpp test/randomTest.cpp)

target_link_libraries(cmakeTest gtest gtest_main)

