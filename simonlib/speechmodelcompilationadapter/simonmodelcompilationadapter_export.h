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

#ifndef SIMON_MODELCOMPILATIONADAPTER_EXPORT_H_D56DDD9BBE944C24A6E2560A75AA881A
#define SIMON_MODELCOMPILATIONADAPTER_EXPORT_H_D56DDD9BBE944C24A6E2560A75AA881A

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef MODELCOMPILATIONADAPTER_EXPORT
# if defined(MAKE_SIMONMODELCOMPILATIONADAPTER_LIB)
// We are building this library
#  define MODELCOMPILATIONADAPTER_EXPORT KDE_EXPORT
# else
#  define MODELCOMPILATIONADAPTER_EXPORT KDE_IMPORT
# endif
#endif

# ifndef MODELCOMPILATIONADAPTER_EXPORT_DEPRECATED
#  define MODELCOMPILATIONADAPTER_EXPORT_DEPRECATED KDE_DEPRECATED MODELCOMPILATIONADAPTER_EXPORT
# endif
#endif
