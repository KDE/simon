# cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

add_subdirectory(src)
add_subdirectory(icons)
# add_subdirectory(cmake)

