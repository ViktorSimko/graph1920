cmake_minimum_required (VERSION 2.6)

project (Graph2019)

add_executable(Graph2019 main.cpp)

target_link_libraries(Graph2019 ${PROJECT_SOURCE_DIR}/lib/libglui.a GL GLU glut)

