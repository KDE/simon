cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
cmake_policy(SET CMP0017 NEW)
cmake_policy(SET CMP0002 OLD)

project(Simon)

ENABLE_TESTING()

set(CMAKE_SIMON_VERSION_MAJOR 0)
set(CMAKE_SIMON_VERSION_MINOR 3)
set(CMAKE_SIMON_VERSION_PATCH 85)
set(CMAKE_SIMON_VERSION_STRING  "${CMAKE_SIMON_VERSION_MAJOR}.${CMAKE_SIMON_VERSION_MINOR}.${CMAKE_SIMON_VERSION_PATCH}")
configure_file(version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(KDE4 "4.4" REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtDbus QtGui QtTest QtXml QtScript QtConcurrent REQUIRED)
find_package(Qwt6 REQUIRED)

macro_optional_find_package(LibSampleRate)
macro_log_feature(LIBSAMPLERATE_FOUND "LibSampleRate" "Resampling library" "http://www.mega-nerd.com/SRC/" FALSE "" "Required to activate resampling.")

include_directories(${CMAKE_BINARY_DIR} .)
if(LIBSAMPLERATE_FOUND)
include_directories(${LIBSAMPLERATE_INCLUDE_DIR})
add_definitions(${LIBSAMPLERATE_DEFINITIONS})
endif(LIBSAMPLERATE_FOUND)

IF(UNIX)
  find_package(ALSA REQUIRED)
  set(plattformLibraries ${ALSA_LIBRARY})
  include_directories(${ALSA_INCLUDE_DIR})
ELSE(UNIX)
  find_package(DirectSound REQUIRED)
  set(plattformLibraries ${DIRECTSOUND_LIBRARY})
  include_directories(${DIRECTSOUND_INCLUDE_DIRS})
ENDIF(UNIX)


find_program(PSPHINX "pocketsphinx_batch")
find_program(PJULIUS "julius")
if(UNIX)
  FIND_LIBRARY(X11_XTest_LIB Xtst ${X11_LIB_SEARCH_PATH})
  FIND_LIBRARY(X11_X11_LIB X11 ${X11_LIB_SEARCH_PATH})
endif(UNIX)

macro_optional_find_package(KdepimLibs 4.5.60)
macro_log_feature(KdepimLibs_FOUND "KDE PIM Libs" "KDE Libraries for PIM" "http://pim.kde.org/" FALSE "4.5.60" "Required for PIM plugin.")
set(USE_PLASMA true CACHE BOOL "Use the plasma data engines in the dialog plugin")

if(USE_PLASMA)
  add_definitions(-DUSE_PLASMA)
endif(USE_PLASMA)

macro_optional_find_package(QAccessibilityClient)
macro_log_feature(QAccessibilityClient_FOUND "qaccessibilityclient" "KDE client-side accessibility library" "https://projects.kde.org/projects/playground/accessibility/libkdeaccessibilityclient" FALSE "" "Required to enable ATSPI plugin.")

find_package(Sphinxbase)
macro_log_feature(SphinxBase_LIBRARIES "Sphinxbase"
  "Open source toolkit for speech recognition" "http://cmusphinx.sourceforge.net/"
  FALSE "" "Required to build Sphinx backend")
find_package(Pocketsphinx)
macro_log_feature(POCKETSPHINX_LIBRARIES "PocketSphinx"
  "PocketSphinx is a small-footprint continuous speech recognition system"
  "http://cmusphinx.sourceforge.net/" FALSE "" 
  "Required to build Pocket Sphinx backend")
find_package(OpenCV)
macro_log_feature(OpenCV_LIBS "OpenCV"
  "OpenCV (Open Source Computer Vision) is a library of programming functions for real time computer vision" "http://http://opencv.willowgarage.com/"
  FALSE "" "Required for Simon Vision")
  
set(BackendType "both" CACHE STRING "Backend type used while building")
set_property(CACHE BackendType PROPERTY STRINGS jhtk both)

if(OpenCV_FOUND)
  link_directories(${OpenCV_LIB_DIR})
  add_definitions(-DWITH_SIMONCV)
  set(WITH_SIMONCV true CACHE BOOL "If SimonCV libraries are present")
endif()

if(SphinxBase_LIBRARIES AND POCKETSPHINX_LIBRARIES)
  set(BackendType both)
else()
  set(BackendType jhtk)
endif()

message(STATUS "BackendType='${BackendType}'")

if(${BackendType} STREQUAL both)
  ADD_DEFINITIONS(-DBACKEND_TYPE_BOTH)
  find_package(Sphinxbase REQUIRED)
  find_package(Pocketsphinx REQUIRED)
  include_directories(${POCKETSPHINX_INCLUDE_DIR} ${SphinxBase_INCLUDE_DIR} ../../simonlib)
endif()
if(${BackendType} STREQUAL jhtk)
  ADD_DEFINITIONS(-DBACKEND_TYPE_JHTK)
endif()

include(KDE4Defaults)
include(SimonDirs)
include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${KDE_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIRS}
  ${Qwt6_INCLUDE_DIR} simonlib)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

if(WIN32)
  # old gcc because of MinGW
  add_definitions(-D__INTERLOCKED_DECLARED)
  if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  endif(MSVC)
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-fexceptions)
endif(CMAKE_COMPILER_IS_GNUCXX)

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

if(USE_PLASMA)
  add_subdirectory(simonoid)
endif()


install(FILES ${CMAKE_BINARY_DIR}/version.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon
  COMPONENT simondevel
)

if(NOT WIN32)
  include(simon_package_settings)
endif(NOT WIN32)

macro_display_feature_log()
