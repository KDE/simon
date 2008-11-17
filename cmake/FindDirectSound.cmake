# - Try to find dsound.dll
# Once done this will define
#
#  DIRECTSOUND_FOUND - system has libsndfile
#  DIRECTSOUND_LIBRARY - Link these to use direct sound

# Copyright (c) 2008, Peter Grasch, <grasch@simon-listens.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.


IF (DIRECTSOUND_LIBRARY)
    SET(DIRECTSOUND_FIND_QUIETLY TRUE)
ENDIF (DIRECTSOUND_LIBRARY)


FIND_LIBRARY(DIRECTSOUND_LIBRARY NAMES dsound_simon PATHS
		${CMAKE_CURRENT_SOURCE_DIR})


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DIRECTSOUND DEFAULT_MSG SENT_LIBRARY DIRECTSOUND_LIBRARY)

if (DIRECTSOUND_FOUND)
	if (NOT DIRECTSOUND_FIND_QUIETLY)
		message(STATUS "Found dsound: ${DIRECTSOUND_LIBRARY}")
	endif (NOT DIRECTSOUND_FIND_QUIETLY)
	
else (DIRECTSOUND_FOUND)
	if (DIRECTSOUND_FIND_QUIETLY)
		message(FATAL_ERROR "Could not find dsound.dll! Please COPY C:/Windows/system32/dsound.dll to ${CMAKE_CURRENT_SOURCE_DIR}, rename it to dsound_simon.dll and try again!")
	endif (DIRECTSOUND_FIND_QUIETLY)
endif (DIRECTSOUND_FOUND)


MARK_AS_ADVANCED(DIRECTSOUND_LIBRARY)

