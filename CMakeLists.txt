cmake_minimum_required(VERSION 3.0.0)
project(firstinit VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(firstinit one.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
#set_property(TARGET target PROPERTY CXX_STANDARD 17)
