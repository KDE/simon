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

#ifndef SIMON_SIMONLISTCOMMAND_EXPORT_H_5F7EB1B436334815AFF6B983332E9521
#define SIMON_SIMONLISTCOMMAND_EXPORT_H_5F7EB1B436334815AFF6B983332E9521

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
#include <KGenericFactory>

#ifndef SIMONLISTCOMMAND_EXPORT
# if defined(MAKE_SIMONLISTCOMMAND_LIB)
// We are building this library
#  define SIMONLISTCOMMAND_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONLISTCOMMAND_EXPORT KDE_IMPORT
# endif
#endif

# ifndef SIMONLISTCOMMAND_EXPORT_DEPRECATED
#  define SIMONLISTCOMMAND_EXPORT_DEPRECATED KDE_DEPRECATED SIMONLISTCOMMAND_EXPORT
# endif
#endif
