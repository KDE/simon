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

#ifndef SIMON_SIMONDCONFIGURATION_EXPORT_H_65AD5ED111734E259F13C9B166245649
#define SIMON_SIMONDCONFIGURATION_EXPORT_H_65AD5ED111734E259F13C9B166245649

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef SIMONDCONFIGURATION_EXPORT
# if defined(MAKE_SIMONDCONFIGURATION_LIB)
// We are building this library
#  define SIMONDCONFIGURATION_EXPORT KDE_EXPORT
# else
// We are using this library
#ifdef Q_OS_WIN
#  define SIMONDCONFIGURATION_EXPORT
#else
#  define SIMONDCONFIGURATION_EXPORT KDE_IMPORT
#endif
# endif
#endif

# ifndef SIMONDCONFIGURATION_EXPORT_DEPRECATED
#  define SIMONDCONFIGURATION_EXPORT_DEPRECATED KDE_DEPRECATED SIMONDCONFIGURATION_EXPORT
# endif
#endif
