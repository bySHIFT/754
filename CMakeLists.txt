cmake_minimum_required(VERSION 3.15)

set(__the_PROJECT_NAME 754)
project(${__the_PROJECT_NAME}
  DESCRIPTION "754: 7分钟内5道4位数乘法全对"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${__the_PROJECT_NAME} src/main.cpp)
