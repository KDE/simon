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

#ifndef SIMON_MODELMANAGEMENT_EXPORT_H_0791F558C23544B3B7F419C871F8222A
#define SIMON_MODELMANAGEMENT_EXPORT_H_0791F558C23544B3B7F419C871F8222A

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef MODELMANAGEMENT_EXPORT
# if defined(MAKE_MODELMANAGEMENT_LIB)
// We are building this library
#  define MODELMANAGEMENT_EXPORT KDE_EXPORT
# else
// We are using this library
//#ifdef Q_OS_WIN
//#  define MODELMANAGEMENT_EXPORT
//#else
#  define MODELMANAGEMENT_EXPORT KDE_IMPORT
//#endif
# endif
#endif

# ifndef MODELMANAGEMENT_EXPORT_DEPRECATED
#  define MODELMANAGEMENT_EXPORT_DEPRECATED KDE_DEPRECATED MODELMANAGEMENT_EXPORT
# endif
#endif
