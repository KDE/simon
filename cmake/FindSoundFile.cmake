# - Try to find libsoundfile
# Once done this will define
#
#  SOUNDFILE_FOUND - system has libsndfile
#  SOUNDFILE_INCLUDE_DIR - the libsndfile include directory
#  SOUNDFILE_LIBRARY - Link these to use libsndfile

# Copyright (c) 2008, Peter Grasch, <grasch@simon-listens.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.


IF (SOUNDFILE_INCLUDE_DIR AND SOUNDFILE_LIBRARY)
    SET(SoundFile_FIND_QUIETLY TRUE)
ENDIF (SOUNDFILE_INCLUDE_DIR AND SOUNDFILE_LIBRARY)

FIND_PATH(SOUNDFILE_INCLUDE_DIR sndfile.h PATHS
		/usr/include
		/usr/local/include
		/opt/include
		/opt/local/include
		C:/Programme/soundfile/include)

FIND_LIBRARY(SOUNDFILE_LIBRARY NAMES sndfile PATHS
		/usr/lib
		/usr/local/lib
		/opt/lib
		/opt/local/lib
		C:/Programme/soundfile/lib)

# handle the QUIETLY and REQUIRED arguments and set Julius_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SOUNDFILE DEFAULT_MSG SENT_LIBRARY SOUNDFILE_LIBRARY SOUNDFILE_INCLUDE_DIR)

if (SOUNDFILE_FOUND)
	if (NOT SoundFile_FIND_QUIETLY)
		message(STATUS "Found libsoundfile: ${SOUNDFILE_LIBRARY}")
	endif (NOT SoundFile_FIND_QUIETLY)
else (SOUNDFILE_FOUND)
	if (SoundFile_FIND_QUIETLY)
		message(FATAL_ERROR "Could not find libsoundfile")
	endif (SoundFile_FIND_QUIETLY)
endif (SOUNDFILE_FOUND)


MARK_AS_ADVANCED(SOUNDFILE_INCLUDE_DIR SOUNDFILE_LIBRARY)

