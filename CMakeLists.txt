cmake_minimum_required(VERSION 3.2)
project(opengl_fun)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

add_executable(opengl_fun ${SOURCES})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++17)

find_package(X11 REQUIRED)
find_package(GLM REQUIRED)
target_link_libraries(opengl_fun ${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR} ${GLM_INCLUDE_DIR})

target_link_libraries(opengl_fun GL GLEW)
