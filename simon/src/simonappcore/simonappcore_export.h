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

#ifndef SIMON_SIMONAPPCORE_EXPORT_H_D36E0FE2AB924B508A030DCF33ACF99E
#define SIMON_SIMONAPPCORE_EXPORT_H_D36E0FE2AB924B508A030DCF33ACF99E

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <KF5/KDELibs4Support/kdemacros.h>

#ifndef SIMONAPPCORE_EXPORT
# if defined(MAKE_SIMONAPPCORE_LIB)
// We are building this library
#  define SIMONAPPCORE_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONAPPCORE_EXPORT KDE_IMPORT
# endif
#endif

# ifndef SIMONAPPCORE_EXPORT_DEPRECATED
#  define SIMONAPPCORE_EXPORT_DEPRECATED KDE_DEPRECATED SIMONAPPCORE_EXPORT
# endif
#endif
