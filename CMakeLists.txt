 cmake_minimum_required(VERSION 2.8.12)
 project(RW_Zad6)

 add_definitions("-std=c++11")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

add_executable(RW_Zad6 main.cpp)
 target_link_libraries(RW_Zad6 ${CONAN_LIBS})
# add_executable(RW_Zad6_Win.exe main.cpp)
#  target_link_libraries(RW_Zad6_Win.exe ${CONAN_LIBS})

file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})