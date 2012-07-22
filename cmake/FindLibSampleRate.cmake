# Copyright (c) 2011, Peter Grasch <grasch@simon-listens.org>
# Copyright (c) 2011, Patrick von Reth, <patrick.vonreth@gmail.com>
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without modification, 
# are permitted provided that the following conditions are met:
# 
#     Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#     Redistributions in binary form must reproduce the above copyright notice, 
#     this list of conditions and the following disclaimer in the documentation 
#     and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# - Find the libsamplerate library
#
# This module defines these variables:
#
#  LIBSAMPLERATE_FOUND
#      True if the libsamplerate library was found
#  LIBSAMPLERATE_LIBRARY
#      The location of the libsamplerate library
#  LIBSAMPLERATE_INCLUDE_DIR
#      The include path of the libsamplerate library
#  LIBSAMPLERATE_DEFINITIONS
#      Preprocessor definitions to define

#
# Find the header file
#
FIND_PATH(LIBSAMPLERATE_INCLUDE_DIR samplerate.h)

#
# Find the library
#
FIND_LIBRARY(LIBSAMPLERATE_LIBRARY samplerate
	NAMES samplerate samplerate-0 libsamplerate-0
    DOC "The libsamplerate library")

IF(LIBSAMPLERATE_INCLUDE_DIR AND LIBSAMPLERATE_LIBRARY)
    SET(LIBSAMPLERATE_FOUND true)
    SET(LIBSAMPLERATE_DEFINITIONS -DHAVE_LIBSAMPLERATE_H)
    SET(LIBSAMPLERATE_INCLUDE_DIRS ${LIBSAMPLERATE_INCLUDE_DIR})
    SET(LIBSAMPLERATE_LIBRARIES    ${LIBSAMPLERATE_LIBRARY})
ENDIF(LIBSAMPLERATE_INCLUDE_DIR AND LIBSAMPLERATE_LIBRARY)

IF(LIBSAMPLERATE_FOUND)
    IF(NOT LIBSAMPLERATE_FIND_QUIETLY)
        MESSAGE(STATUS "Found libsamplerate: ${LIBSAMPLERATE_LIBRARY}")
    ENDIF(NOT LIBSAMPLERATE_FIND_QUIETLY)
ELSE(LIBSAMPLERATE_FOUND) 
    IF(LIBSAMPLERATE_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find the libsamplerate")
    ENDIF(LIBSAMPLERATE_FIND_REQUIRED)
ENDIF(LIBSAMPLERATE_FOUND)
