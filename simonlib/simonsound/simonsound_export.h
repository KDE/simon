/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SIMONSOUND_EXPORT_H_DAA46318B9EE4B9888F8F8C2CDDFD6DC
#define SIMON_SIMONSOUND_EXPORT_H_DAA46318B9EE4B9888F8F8C2CDDFD6DC

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <KDELibs4Support/kdemacros.h>

#ifndef SIMONSOUND_EXPORT
# if defined(MAKE_SIMONSOUND_LIB)
// We are building this library
#  define SIMONSOUND_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONSOUND_EXPORT
# endif
#endif

# ifndef SIMONSOUND_EXPORT_DEPRECATED
#  define SIMONSOUND_EXPORT_DEPRECATED KDE_DEPRECATED SIMONSOUND_EXPORT
# endif
#endif
