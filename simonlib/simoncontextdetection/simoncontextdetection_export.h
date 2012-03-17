/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_SIMONCONTEXTDETECTION_EXPORT_H_F1032DFE61FA42E3B31933515AF33099
#define SIMON_SIMONCONTEXTDETECTION_EXPORT_H_F1032DFE61FA42E3B31933515AF33099

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
#include <KDE/KGenericFactory>

#ifndef SIMONCONTEXTDETECTION_EXPORT
# if defined(MAKE_SIMONCONTEXTDETECTION_LIB)
// We are building this library
#  define SIMONCONTEXTDETECTION_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONCONTEXTDETECTION_EXPORT
# endif
#endif

# ifndef SIMONCONTEXTDETECTION_EXPORT_DEPRECATED
#  define SIMONCONTEXTDETECTION_EXPORT_DEPRECATED KDE_DEPRECATED SIMONCONTEXTDETECTION_EXPORT
# endif
#endif
