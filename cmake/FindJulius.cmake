# - Try to find Julius
# Once done this will define
#
#  JULIUS_FOUND - system has Julius
#  JULIUS_INCLUDE_DIR - the Julius include directory
#  JULIUS_LIBRARIES - Link these to use Julius

# Copyright (c) 2008, Peter Grasch, <grasch@simon-listens.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.


IF (JULIUS_INCLUDE_DIR AND JULIUS_LIBRARIES)
    SET(Julius_FIND_QUIETLY TRUE)
ENDIF (JULIUS_INCLUDE_DIR AND JULIUS_LIBRARIES)

FIND_PATH(JULIUS_INCLUDE_DIR julius/julius.h PATHS
		/usr/include
		/usr/local/include
		/opt/include
		/opt/local/include
		C:/Programme/Julius/include)

FIND_LIBRARY(SENT_LIBRARY NAMES sent PATHS
		/usr/lib
		/usr/local/lib
		/opt/lib
		/opt/local/lib
		C:/Programme/Julius/lib)
FIND_LIBRARY(JULIUS_LIBRARY NAMES julius PATHS
		/usr/lib
		/usr/local/lib
		/opt/lib
		/opt/local/lib
		C:/Programme/Julius/lib)

# handle the QUIETLY and REQUIRED arguments and set Julius_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Julius DEFAULT_MSG SENT_LIBRARY JULIUS_LIBRARY JULIUS_INCLUDE_DIR)

SET (JULIUS_LIBRARIES ${SENT_LIBRARY} ${JULIUS_LIBRARY} -lsent -ljulius)

if (JULIUS_FOUND)
	if (NOT Julius_FIND_QUIETLY)
		message(STATUS "Found Julius: ${JULIUS_LIBRARIES}")
	endif (NOT Julius_FIND_QUIETLY)
else (JULIUS_FOUND)
	if (Julius_FIND_QUIETLY)
		message(FATAL_ERROR "Could not find Julius")
	endif (Julius_FIND_QUIETLY)
endif (JULIUS_FOUND)


MARK_AS_ADVANCED(JULIUS_INCLUDE_DIR JULIUS_LIBRARIES)

