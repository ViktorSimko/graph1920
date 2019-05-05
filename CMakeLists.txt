
cmake_minimum_required (VERSION 2.8.12)
project (Graph2019)

set(CMAKE_BUILD_TYPE Debug)

find_package (Qt5Widgets)
find_package (Qt5OpenGL)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

#set (Graph2019_src ${PROJECT_SOURCE_DIR}/mainwindow.cpp)
#set (Graph2019_hdr ${PROJECT_SOURCE_DIR}/mainwindow.h)
#set (Graph2019_ui  ${PROJECT_SOURCE_DIR}/mainwindow.ui)

set (Graph2019Bin_src ${PROJECT_SOURCE_DIR}/main_qt.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/mainwidget.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/oglwidget.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/graph_engine.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyData.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyData.h)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyElement.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyElement.h)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyFormat.h)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyProperty.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyProperty.h)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyReader.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyReader.h)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyWriter.cpp)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/third_party_libs/easy_as_ply/PlyWriter.h)

qt5_wrap_cpp(Graph2019_mainwidget_moc ${PROJECT_SOURCE_DIR}/mainwidget.h)
list (APPEND Graph2019Bin_src ${Graph2019_mainwidget_moc})
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/oglwidget.h)
list (APPEND Graph2019Bin_src ${PROJECT_SOURCE_DIR}/graph_engine.h)

#qt5_wrap_ui (Graph2019_ui_moc  ${MyProjectLib_ui})

include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

#add_library (Graph2019Lib SHARED
#    ${Graph2019_src}
#    ${Graph2019_hdr_moc}
#    ${Graph2019_ui_moc}
#)
#target_link_libraries (Graph2019Lib Qt5::Widgets)

add_executable(Graph2019 ${Graph2019Bin_src})
target_link_libraries (Graph2019 Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

