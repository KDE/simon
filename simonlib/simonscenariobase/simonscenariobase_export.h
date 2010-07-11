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

#ifndef SIMON_SCENARIOBASE_EXPORT_H_85CC537B8A5542B699807296D0561722
#define SIMON_SCENARIOBASE_EXPORT_H_85CC537B8A5542B699807296D0561722

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>

#ifndef SCENARIOBASE_EXPORT
# if defined(MAKE_SCENARIOBASE_LIB)
// We are building this library
#  define SCENARIOBASE_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SCENARIOBASE_EXPORT KDE_IMPORT
# endif
#endif

# ifndef SCENARIOBASE_EXPORT_DEPRECATED
#  define SCENARIOBASE_EXPORT_DEPRECATED KDE_DEPRECATED SCENARIOBASE_EXPORT
# endif
#endif
