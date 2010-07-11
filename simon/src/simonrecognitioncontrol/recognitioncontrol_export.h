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

#ifndef SIMON_RECOGNITIONCONTROL_EXPORT_H_C1550CB9C9C9437BB1201CF606DFB76C
#define SIMON_RECOGNITIONCONTROL_EXPORT_H_C1550CB9C9C9437BB1201CF606DFB76C

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef RECOGNITIONCONTROL_EXPORT
# if defined(MAKE_RECOGNITIONCONTROL_LIB)
// We are building this library
#  define RECOGNITIONCONTROL_EXPORT KDE_EXPORT
# else
// We are using this library
//#ifdef Q_OS_WIN
//#  define RECOGNITIONCONTROL_EXPORT
//#else
#  define RECOGNITIONCONTROL_EXPORT KDE_IMPORT
//#endif
# endif
#endif

# ifndef RECOGNITIONCONTROL_EXPORT_DEPRECATED
#  define RECOGNITIONCONTROL_EXPORT_DEPRECATED KDE_DEPRECATED RECOGNITIONCONTROL_EXPORT
# endif
#endif
