cmake_minimum_required(VERSION 2.8.12)
cmake_policy(SET CMP0017 NEW)
cmake_policy(SET CMP0002 OLD)
cmake_policy(SET CMP0026 OLD)
cmake_policy(SET CMP0022 OLD)

project(Simon)

set(CMAKE_SIMON_VERSION_MAJOR 0)
set(CMAKE_SIMON_VERSION_MINOR 5)
set(CMAKE_SIMON_VERSION_PATCH 0)
set(CMAKE_SIMON_VERSION_STRING  "${CMAKE_SIMON_VERSION_MAJOR}.${CMAKE_SIMON_VERSION_MINOR}.${CMAKE_SIMON_VERSION_PATCH}")
configure_file(version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

find_package(ECM 1.7.0 REQUIRED NOMODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR}  ${CMAKE_MODULE_PATH})

find_package(Qt5 5.3.0 REQUIRED COMPONENTS Core Widgets Sql DBus Gui Test Xml Script Concurrent X11Extras Network)
find_package(KF5 5.7.0 REQUIRED COMPONENTS Archive WidgetsAddons I18n KHtml
   KIO NewStuff Config Codecs KDELibs4Support KCMUtils Sonnet Plasma IconThemes WidgetsAddons)

find_package(Qwt6)
set_package_properties(Qwt6 PROPERTIES TYPE REQUIRED
   DESCRIPTION "Qwt Widget Library (2D plotting, sliders etc.)"
   URL "http://qwt.sourceforge.net/"
   PURPOSE "Required for displaying diagrams in simon")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

include(ECMInstallIcons)
include(KDEUIMacros)
include(ECMAddAppIcon)
include(GenerateExportHeader)


find_package(LibSampleRate)
set_package_properties(LibSampleRate PROPERTIES TYPE OPTIONAL DESCRIPTION "Resampling library" URL "http://www.mega-nerd.com/SRC/" PURPOSE "Required to activate resampling.")

include_directories(${CMAKE_BINARY_DIR} .)
if(LIBSAMPLERATE_FOUND)
include_directories(${LIBSAMPLERATE_INCLUDE_DIR})
add_definitions(${LIBSAMPLERATE_DEFINITIONS})
endif(LIBSAMPLERATE_FOUND)

IF(UNIX AND NOT APPLE AND NOT WIN32)
  find_package(ALSA REQUIRED)
  set(plattformLibraries ${ALSA_LIBRARY})
  include_directories(${ALSA_INCLUDE_DIR})
ENDIF()

IF (WIN32)
  find_package(DirectSound REQUIRED)
  set(plattformLibraries ${DIRECTSOUND_LIBRARY})
  include_directories(${DIRECTSOUND_INCLUDE_DIRS})
ENDIF()


find_program(PSPHINX "pocketsphinx_batch")
find_program(PJULIUS "julius")
if(UNIX)
  FIND_LIBRARY(X11_XTest_LIB Xtst ${X11_LIB_SEARCH_PATH})
  FIND_LIBRARY(X11_X11_LIB X11 ${X11_LIB_SEARCH_PATH})
endif(UNIX)

find_package(KF5CalendarCore)
set_package_properties(KF5CalendarCore PROPERTIES TYPE OPTIONAL DESCRIPTION "KDE calendar access library" URL "https://api.kde.org/kdepim/index.html" PURPOSE "Required for PIM plugin.")
find_package(KF5Akonadi)
set_package_properties(KF5Akonadi PROPERTIES TYPE OPTIONAL DESCRIPTION "KDE PIM storage framework" URL "https://api.kde.org/kdepim/index.html" PURPOSE "Required for PIM plugin.")

if(UNIX AND NOT APPLE)
  # set(USE_PLASMA true CACHE BOOL "Use the plasma data engines in the dialog plugin")
  set(USE_PLASMA false CACHE BOOL "Plasma data engines have not been ported.")
else()
  set(USE_PLASMA false)
endif()

if(USE_PLASMA)
  add_definitions(-DUSE_PLASMA)
endif(USE_PLASMA)

find_package(QAccessibilityClient QUIET)
set_package_properties(QAccessibilityClient PROPERTIES TYPE OPTIONAL
   DESCRIPTION "KDE client-side accessibility library"
   URL "https://projects.kde.org/projects/playground/accessibility/libkdeaccessibilityclient"
   PURPOSE "Required to enable ATSPI plugin..")

find_package(SphinxBase)
set_package_properties(SphinxBase PROPERTIES TYPE OPTIONAL
   DESCRIPTION "Open source toolkit for speech recognition"
   URL "http://cmusphinx.sourceforge.net/"
   PURPOSE "Required to build Sphinx backend")
find_package(Pocketsphinx)
set_package_properties(Pocketsphinx PROPERTIES TYPE OPTIONAL
      DESCRIPTION "PocketSphinx is a small-footprint continuous speech recognition system"
      URL "http://cmusphinx.sourceforge.net/"
      PURPOSE "Required to build Pocket Sphinx backend")
find_package(OpenCV)
set_package_properties(OpenCV PROPERTIES TYPE OPTIONAL
   DESCRIPTION "OpenCV (Open Source Computer Vision) is a library of programming functions for real time computer vision"
   URL "http://http://opencv.willowgarage.com/"
   PURPOSE "Required for Simon Vision")

find_package(VRPN)
set_package_properties(VRPN PROPERTIES TYPE OPTIONAL
   DESCRIPTION "Virtual-Reality Peripheral Network Connector"
   URL "http://www.cs.unc.edu/Research/vrpn/index.html"
   PURPOSE "Required to build the VRPN command plugin")

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
  find_package(SphinxBase REQUIRED)
  find_package(Pocketsphinx REQUIRED)
  include_directories(${POCKETSPHINX_INCLUDE_DIR}/pocketsphinx ${SphinxBase_INCLUDE_DIR} ${SphinxBase_INCLUDE_DIR}/sphinxbase ../../simonlib)
endif()
if(${BackendType} STREQUAL jhtk)
  ADD_DEFINITIONS(-DBACKEND_TYPE_JHTK)
endif()

include(SimonDirs)
include_directories(${Qwt6_INCLUDE_DIR} simonlib)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

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

string(FIND "/usr/include/KDE" match "${INCLUDE_DIRECTORIES}" )
if(match)
  message(FATAL_ERROR "/usr/include/KDE was found.  Bad!")
endif()
message(STATUS "Include Directories: ${INCLUDE_DIRECTORIES}")

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
  # QT5TODO: Port this to Plasma 5
  # add_subdirectory(simonoid)
endif()


install(FILES ${CMAKE_BINARY_DIR}/version.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon
  COMPONENT simondevel
)

if(NOT WIN32)
  include(simon_package_settings)
endif(NOT WIN32)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
