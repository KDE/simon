cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0017 NEW)
cmake_policy(SET CMP0002 OLD)
project(simon)

ENABLE_TESTING()

set(CMAKE_SIMON_VERSION_MAJOR 0)
set(CMAKE_SIMON_VERSION_MINOR 3)
set(CMAKE_SIMON_VERSION_PATCH 80)
set(CMAKE_SIMON_VERSION_STRING  "${CMAKE_SIMON_VERSION_MAJOR}.${CMAKE_SIMON_VERSION_MINOR}.${CMAKE_SIMON_VERSION_PATCH}")
configure_file(version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(KDE4 "4.4" REQUIRED)

#add_definitions(-DUSPHINX)

if(WIN32)
# old gcc because of MinGW
#add_definitions(-std=c++98 -D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
add_definitions(-D__INTERLOCKED_DECLARED)
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)
else(WIN32)
	#add_definitions(-pedantic-errors -std=c++98 -Wno-variadic-macros)
endif(WIN32)

include_directories( ${KDE4_INCLUDES} )

find_program( PSPHINX "pocketsphinx_batch" )
find_program( PJULIUS "julius" )

find_package(Sphinxbase)
find_package(Pocketsphinx)

#if(SphinxBase_LIBRARIES)
#message(${SphinxBase_LIBRARIES})
#endif(SphinxBase_LIBRARIES)

#if(POCKETSPHINX_LIBRARIES)
#message(${POCKETSPHINX_LIBRARIES})
#endif(POCKETSPHINX_LIBRARIES)

set(BackendType "both" CACHE STRING "Backend type used while building")
set_property(CACHE BackendType PROPERTY STRINGS jhtk both)

if(SphinxBase_LIBRARIES AND POCKETSPHINX_LIBRARIES)
    set(BackendType both)
else()
    set(BackendType jhtk)
endif()


message(STATUS "BackendType='${BackendType}'")
if(${BackendType} STREQUAL both)
ADD_DEFINITIONS(-DBACKEND_TYPE_BOTH)
endif(${BackendType} STREQUAL both)

if(${BackendType} STREQUAL jhtk)
ADD_DEFINITIONS(-DBACKEND_TYPE_JHTK)
endif(${BackendType} STREQUAL jhtk)

add_subdirectory(simonlib)
add_subdirectory(simon)
add_subdirectory(simond)
add_subdirectory(ksimond)
add_subdirectory(plugins)
add_subdirectory(sam)
add_subdirectory(ssc)
add_subdirectory(sscd)
add_subdirectory(afaras)
add_subdirectory(doc)

if (USE_PLASMA)
add_subdirectory(simonoid)
endif (USE_PLASMA)


install( FILES ${CMAKE_BINARY_DIR}/version.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon
  COMPONENT simondevel
)


# if(WIN32)
# add_subdirectory(package/KDE)
# ENDIF(WIN32)
if(NOT WIN32)
include(simon_package_settings)
endif(NOT WIN32)

