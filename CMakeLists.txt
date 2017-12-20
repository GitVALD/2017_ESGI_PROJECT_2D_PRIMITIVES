cmake_minimum_required(VERSION 3.8)
project(2017_ESGI_PROJECT_2D_PRIMITIVES)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(2017_ESGI_PROJECT_2D_PRIMITIVES ${SOURCE_FILES})


find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)



include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})