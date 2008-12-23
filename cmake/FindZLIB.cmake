#
# Find the native ZLIB includes and library
#

#
# Look for an installation or build tree.
#
# Look in standard UNIX install locations
#
FIND_PATH(ZLIB_INCLUDE_DIR zlib.h /usr/include/zlib /usr/local/include/zlib)

FIND_LIBRARY(ZLIB_LIBRARY NAMES zlib PATH /usr/lib /usr/local/lib) 

IF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)
   SET(ZLIB_FOUND TRUE)
ENDIF (ZLIB_INCLUDE_DIR AND ZLIB_LIBRARY)


IF (ZLIB_FOUND)
   IF (NOT ZLIB_FIND_QUIETLY)
      MESSAGE(STATUS "Found ZLIB: ${ZLIB_LIBRARY}")
   ENDIF (NOT ZLIB_FIND_QUIETLY)
ELSE (ZLIB_FOUND)
   IF (ZLIB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find ZLIB")
   ENDIF (ZLIB_FIND_REQUIRED)
ENDIF (ZLIB_FOUND)

