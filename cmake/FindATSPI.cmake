# Copyright (c) 2008, Peter Grasch <grasch@simon-listens.org>
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

FIND_PATH(ATSPI_INCLUDE_DIR atspi/atspi.h PATH_SUFFIXES at-spi-2.0)

FIND_LIBRARY(ATSPI_LIBRARY NAMES atspi PATH /usr/lib /usr/local/lib) 

IF (ATSPI_INCLUDE_DIR AND ATSPI_LIBRARY)
   SET(ATSPI_FOUND TRUE)
ENDIF (ATSPI_INCLUDE_DIR AND ATSPI_LIBRARY)


IF (ATSPI_FOUND)
   IF (NOT ATSPI_FIND_QUIETLY)
      MESSAGE(STATUS "Found ATSPI: ${ATSPI_LIBRARY}")
   ENDIF (NOT ATSPI_FIND_QUIETLY)
ELSE (ATSPI_FOUND)
   IF (ATSPI_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find ATSPI")
   ENDIF (ATSPI_FIND_REQUIRED)
ENDIF (ATSPI_FOUND)


