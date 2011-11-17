# - Try to find dsound.dll
# Once done this will define
#
#  DIRECTSOUND_FOUND - system has direct sound
#  DIRECTSOUND_INCLUDE_DIR - include directory
#  DIRECTSOUND_LIBRARY - Link these to use direct sound

# Copyright (c) 2011, Patrick von Reth, <patrick.vonreth@gmail.com>
# Copyright (c) 2008-2011, Peter Grasch, <grasch@simon-listens.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.

set(DIRECTSOUND_INCLUDE_DIRS $ENV{DXSDK_DIR}/include $ENV{DXSDK_DIR})

if(MSVC)
    FIND_LIBRARY(_DSOUD_LIB NAMES dsound PATHS
        $ENV{DXSDK_DIR}/Lib/x86)

    FIND_LIBRARY(_DXGUID_LIB NAMES dxguid PATHS
        $ENV{DXSDK_DIR}/Lib/x86) #TODO:better esearch path ant cosinder achitecture
    SET (DIRECTSOUND_LIBRARY ${_DSOUD_LIB} ${_DXGUID_LIB} )
else(MSVC)
    set (DIRECTSOUND_LIBRARY dsound dxguid)
endif(MSVC)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DIRECTSOUND DEFAULT_MSG DIRECTSOUND_LIBRARY)

if (DIRECTSOUND_FOUND)
	if (NOT DIRECTSOUND_FIND_QUIETLY)
		message(STATUS "Found dsound: ${DIRECTSOUND_LIBRARY}")
	endif (NOT DIRECTSOUND_FIND_QUIETLY)
	
else (DIRECTSOUND_FOUND)
	if (DIRECTSOUND_FIND_QUIETLY)
		message(FATAL_ERROR "Could not find dsound! Please install the directx sdk http://msdn.microsoft.com/en-us/directx/aa937788")
	endif (DIRECTSOUND_FIND_QUIETLY)
endif (DIRECTSOUND_FOUND)


MARK_AS_ADVANCED(DIRECTSOUND_LIBRARY)

