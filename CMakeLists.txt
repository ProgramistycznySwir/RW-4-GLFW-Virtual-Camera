 cmake_minimum_required(VERSION 2.8.12)
 project(RW_Zad3)

 add_definitions("-std=c++11")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 add_executable(RW_Zad3 main.c)
 target_link_libraries(RW_Zad3 ${CONAN_LIBS})