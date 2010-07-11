/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SIMONPROGRESSTRACKING_EXPORT_H_1250BD8FD4E949E1B720AF1D9D49282E
#define SIMON_SIMONPROGRESSTRACKING_EXPORT_H_1250BD8FD4E949E1B720AF1D9D49282E

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef SIMONPROGRESSTRACKING_EXPORT
# if defined(MAKE_SIMONPROGRESSTRACKING_LIB)
// We are building this library
#  define SIMONPROGRESSTRACKING_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONPROGRESSTRACKING_EXPORT
# endif
#endif

# ifndef SIMONPROGRESSTRACKING_EXPORT_DEPRECATED
#  define SIMONPROGRESSTRACKING_EXPORT_DEPRECATED KDE_DEPRECATED SIMONPROGRESSTRACKING_EXPORT
# endif
#endif
