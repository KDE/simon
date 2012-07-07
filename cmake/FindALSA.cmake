# - Find alsa
# Find the alsa libraries (asound)
#
#  This module defines the following variables:
#     ALSA_FOUND       - True if ALSA_INCLUDE_DIR & ALSA_LIBRARY are found
#     ALSA_LIBRARIES   - Set when ALSA_LIBRARY is found
#     ALSA_INCLUDE_DIRS - Set when ALSA_INCLUDE_DIR is found
#
#     ALSA_INCLUDE_DIR - where to find asoundlib.h, etc.
#     ALSA_LIBRARY     - the asound library
#

#=============================================================================
# Copyright 2009 Kitware, Inc.
# Copyright 2009 Philip Lowman <philip@yhbt.com>
#
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions
#are met:
#
#* Redistributions of source code must retain the above copyright
#  notice, this list of conditions and the following disclaimer.
#
#* Redistributions in binary form must reproduce the above copyright
#  notice, this list of conditions and the following disclaimer in the
#  documentation and/or other materials provided with the distribution.
#
#* Neither the names of Kitware, Inc., the Insight Software Consortium,
#  nor the names of their contributors may be used to endorse or promote
#  products derived from this software without specific prior written
#  permission.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#------------------------------------------------------------------------------
#
#The above copyright and license notice applies to distributions of
#CMake in source and binary form.  Some source files contain additional
#notices of original copyright by their contributors; see each source
#for details.  Third-party software packages supplied with CMake under
#compatible licenses provide their own copyright notices documented in
#corresponding subdirectories.
#
#------------------------------------------------------------------------------
#
#CMake was initially developed by Kitware with the following sponsorship:
#
# * National Library of Medicine at the National Institutes of Health
#   as part of the Insight Segmentation and Registration Toolkit (ITK).
#
# * US National Labs (Los Alamos, Livermore, Sandia) ASC Parallel
#   Visualization Initiative.
#
# * National Alliance for Medical Image Computing (NAMIC) is funded by the
#   National Institutes of Health through the NIH Roadmap for Medical Research,
#   Grant U54 EB005149.
#
# * Kitware, Inc.

find_path(ALSA_INCLUDE_DIR NAMES asoundlib.h
          PATH_SUFFIXES alsa
          DOC "The ALSA (asound) include directory"
)

find_library(ALSA_LIBRARY NAMES asound
          DOC "The ALSA (asound) library"
)

# handle the QUIETLY and REQUIRED arguments and set ALSA_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ALSA DEFAULT_MSG ALSA_LIBRARY ALSA_INCLUDE_DIR)

if(ALSA_FOUND)
  set( ALSA_LIBRARIES ${ALSA_LIBRARY} )
  set( ALSA_INCLUDE_DIRS ${ALSA_INCLUDE_DIR} )
endif()

mark_as_advanced(ALSA_INCLUDE_DIR ALSA_LIBRARY)
