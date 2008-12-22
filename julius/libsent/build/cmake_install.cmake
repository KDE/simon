# Install script for directory: /home/bedahr/Daten/simon/svn/trunk/julius/libsent

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Devel)$")
  FILE(INSTALL DESTINATION "/usr/local/include/sent" TYPE FILE FILES
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/config.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/htk_defs.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/adin.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/stddefs.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/dfa.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/util.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/htk_hmm.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/vocabulary.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/hmm_calc.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/ptree.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/mfcc.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/hmm.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/machines.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/ngram2.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/speech.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/htk_param.h"
    "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/./include/sent/tcpip.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Devel)$")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    IF(EXISTS "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION")
      FILE(RPATH_CHECK
           FILE "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION"
           RPATH "/usr/local/lib64")
    ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION")
    FILE(INSTALL DESTINATION "/usr/local/lib64" TYPE SHARED_LIBRARY FILES
      "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/build/lib/libsent.so.SOVERSION"
      "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/build/lib/libsent.so"
      )
    IF(EXISTS "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION")
      FILE(RPATH_CHANGE
           FILE "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION"
           OLD_RPATH "::::::::::::::::"
           NEW_RPATH "/usr/local/lib64")
      IF(CMAKE_INSTALL_DO_STRIP)
        EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION")
      ENDIF(CMAKE_INSTALL_DO_STRIP)
    ENDIF(EXISTS "$ENV{DESTDIR}/usr/local/lib64/libsent.so.SOVERSION")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/bedahr/Daten/simon/svn/trunk/julius/libsent/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
