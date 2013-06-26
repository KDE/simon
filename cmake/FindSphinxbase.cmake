# Copyright (c) 2012, Vladislav Sitalo <root@stvad.org>
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

include(LibFindMacros)

# Dependencies

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(SphinxBase_PKGCONF sphinxbase)

# Include dir
find_path(SphinxBase_INCLUDE_DIR
  NAMES cmd_ln.h
  PATHS ${SphinxBase_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES sphinxbase
)

# Finally the library itself
find_library(SphinxBase_LIBRARY
  NAMES sphinxbase sphinxad
  PATHS ${SphinxBase_PKGCONF_LIBRARY_DIRS}
)

IF(WIN32)
  set (SphinxBase_LIBRARIES ${SphinxBase_LIBRARIES} iconv)
ENDIF(WIN32)


# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this lib depends on.
set(SphinxBase_PROCESS_INCLUDES SphinxBase_INCLUDE_DIR SphinxBase_INCLUDE_DIRS)
set(SphinxBase_PROCESS_LIBS SphinxBase_LIBRARY SphinxBase_LIBRARIES)
libfind_process(SphinxBase)

#include(FindPackageHandleStandardArgs)
#if(SPHINXBASE_INCLUDE_DIR AND SPHINXBASE_LIBRARIES)
#set(Sphinxbase_FIND_QUIETLY TRUE)
#endif(SPHINXBASE_INCLUDE_DIR AND SPHINXBASE_LIBRARIES)

#FIND_PATH(SPHINXBASE_INCLUDE_DIR cmd_ln.h PATHS /usr/include/sphinxbase)
#find_library(SPHINXBASE_LIBRARIES sphinxad sphinxbase PATHS /usr/lib)
#find_package_handle_standard_args(Sphinxbase DEFAULT_MSG SPHINXBASE_LIBRARIES SPHINXBASE_INCLUDE_DIR)
