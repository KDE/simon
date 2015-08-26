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

#ifndef SIMON_MODELCOMPILATIONMANAGEMENT_EXPORT_H_F214FE26AC8347EFB122602AB01F18FA
#define SIMON_MODELCOMPILATIONMANAGEMENT_EXPORT_H_F214FE26AC8347EFB122602AB01F18FA

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <KF5/KDELibs4Support/kdemacros.h>

#ifndef MODELCOMPILATIONMANAGEMENT_EXPORT
# if defined(MAKE_SIMONMODELCOMPILATION_LIB)
// We are building this library
#  define MODELCOMPILATIONMANAGEMENT_EXPORT KDE_EXPORT
# else
#  define MODELCOMPILATIONMANAGEMENT_EXPORT KDE_IMPORT
# endif
#endif

# ifndef MODELCOMPILATIONMANAGEMENT_EXPORT_DEPRECATED
#  define MODELCOMPILATIONMANAGEMENT_EXPORT_DEPRECATED KDE_DEPRECATED MODELCOMPILATIONMANAGEMENT_EXPORT
# endif
#endif
