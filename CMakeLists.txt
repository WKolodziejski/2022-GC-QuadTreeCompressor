cmake_minimum_required(VERSION 3.13)
project(enc)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
set(SOURCE_FILES encoder.cpp decoder.cpp)
add_executable(enc encoder.cpp tree.cpp tree.h)
add_executable(dec decoder.cpp tree.cpp tree.h)
include_directories(/usr/local/bin)
target_link_libraries(enc ${OpenCV_LIBS})
target_link_libraries(dec ${OpenCV_LIBS})