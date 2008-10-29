project(simon)
find_package(KDE4 REQUIRED)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

add_subdirectory(simonlib)
#add_subdirectory(juliusd_old)
add_subdirectory(simon)
add_subdirectory(simond)
add_subdirectory(plugins)
