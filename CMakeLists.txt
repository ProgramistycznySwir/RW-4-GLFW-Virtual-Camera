 cmake_minimum_required(VERSION 2.8.12)
 project(RW_Zad5)

 add_definitions("-std=c++11")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

add_executable(RW_Zad5 main.c)
 target_link_libraries(RW_Zad5 ${CONAN_LIBS})
add_executable(RW_Zad5_Win.exe main.c)
 target_link_libraries(RW_Zad5_Win.exe ${CONAN_LIBS})
# add_executable(OdRadka odRadka.cpp)
#   target_link_libraries(OdRadka ${CONAN_LIBS})

# set(RESOURCE_FILES
#   Resources/Shaders/diffuse.frag
#   Resources/Shaders/diffuse.vert
#   Resources/Shaders/phong.frag
#   Resources/Shaders/phong.vert
#   Resources/Shaders/specular.frag
#   Resources/Shaders/specular.vert
#   Resources/Shaders/textureBase.frag
#   Resources/Shaders/textureBase.vert
#   Resources/Shaders/textureLight.frag
#   Resources/Shaders/textureLight.vert
#   Resources/container.jpg
#   )

# add_custom_command(TARGET RW_Zad5 POST_BUILD
#                 COMMAND ${CMAKE_COMMAND} -E copy_directory
#                 ${CMAKE_SOURCE_DIR}/Resources/ $<TARGET_FILE_DIR:RW_Zad5>)
file(COPY Resources DESTINATION ${CMAKE_BINARY_DIR})
# copy_directory

# set_target_properties(RW_Zad5 PROPERTIES
#     RESOURCE "${RESOURCE_FILES}")
# set_target_properties(RW_Zad5_Win.exe PROPERTIES
#     RESOURCE "${RESOURCE_FILES}")