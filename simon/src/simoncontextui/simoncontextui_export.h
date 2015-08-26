/*
 *   Copyright (C) 2011 Adam Nash <adan.t.nash@gmail.com>
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

#ifndef SIMON_SIMONCONTEXTUI_EXPORT_H_A1403E4D54AD48B5BAE6194302B00576
#define SIMON_SIMONCONTEXTUI_EXPORT_H_A1403E4D54AD48B5BAE6194302B00576

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <KF5/KDELibs4Support/kdemacros.h>

#ifndef SIMONCONTEXTUI_EXPORT
# if defined(MAKE_SIMONCONTEXTUI_LIB)
// We are building this library
#  define SIMONCONTEXTUI_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONCONTEXTUI_EXPORT
# endif
#endif

# ifndef SIMONCONTEXTUI_EXPORT_DEPRECATED
#  define SIMONCONTEXTUI_EXPORT_DEPRECATED KDE_DEPRECATED SIMONCONTEXTUI_EXPORT
# endif
#endif
