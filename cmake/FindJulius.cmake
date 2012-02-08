# - Try to find Julius
# Once done this will define
#
#  JULIUS_FOUND - system has Julius
#  JULIUS_INCLUDES - the Julius include directory
#  JULIUS_LIBRARIES - Link these to use Julius

# Copyright (c) 2008-2012, Peter Grasch, <grasch@simon-listens.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.

IF (JULIUS_INCLUDES AND JULIUS_LIBRARIES)
    SET(Julius_FIND_QUIETLY TRUE)
ENDIF (JULIUS_INCLUDES AND JULIUS_LIBRARIES)

FIND_PATH(LIBJULIUS_INCLUDE_DIR  julius.h /usr/include/julius /usr/local/include/julius)
FIND_PATH(LIBSENT_INCLUDE_DIR  adin.h /usr/include/sent /usr/local/include/sent)

FIND_LIBRARY(JULIUS_LIBRARY NAMES julius PATH /usr/lib /usr/local/lib /usr/lib32 /usr/lib64 /usr/local/lib32 /usr/local/lib64) 
FIND_LIBRARY(SENT_LIBRARY NAMES sent PATH /usr/lib /usr/local/lib /usr/lib32 /usr/lib64 /usr/local/lib32 /usr/local/lib64) 

# handle the QUIETLY and REQUIRED arguments and set Julius_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Julius DEFAULT_MSG SENT_LIBRARY JULIUS_LIBRARY LIBJULIUS_INCLUDE_DIR LIBSENT_INCLUDE_DIR)

SET (JULIUS_INCLUDES  ${LIBJULIUS_INCLUDE_DIR} ${LIBSENT_INCLUDE_DIR})
SET (JULIUS_LIBRARIES ${SENT_LIBRARY} ${JULIUS_LIBRARY})

if (JULIUS_FOUND)
	if (NOT Julius_FIND_QUIETLY)
		message(STATUS "Found Julius: ${JULIUS_LIBRARIES}")
	endif (NOT Julius_FIND_QUIETLY)
else (JULIUS_FOUND)
	if (Julius_FIND_QUIETLY)
		message(FATAL_ERROR "Could not find Julius")
	endif (Julius_FIND_QUIETLY)
endif (JULIUS_FOUND)


MARK_AS_ADVANCED(JULIUS_INCLUDES JULIUS_LIBRARIES)

