# cmake 的最小版本要求
cmake_minimum_required(VERSION 3.12)
# 这个CMakeLists管理的工程名称
project(cmakeTest)
set(CMAKE_CXX_STANDARD 14)
#生成可执行程序 语法:add_executable(可执行程序名 要编译的cpp)
<<<<<<< HEAD
add_executable(cmakeTest main.cpp bugReport.h paramParser.h engine.cpp engine.h)
=======
add_executable(cmakeTest main.cpp main.cpp bugReport.h paramParser.h engine.cpp engine.h output.h bugReport.cpp paramParser.cpp)
>>>>>>> bf216d5393ca4be274dc18c3658a7ba634c6871d
