project (simon)

cmake_minimum_required(VERSION 2.6)
include(FindKDE4)
find_package(KDE4 REQUIRED)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(icons)
add_subdirectory(themes)
add_subdirectory(conf)
add_subdirectory(model)
add_subdirectory(texts)
