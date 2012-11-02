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

#ifndef SIMON_SSCD_BASE_DIRECTORY_H_C43F1E0DA7CC438A8A7E90E43CFDCFEA
#define SIMON_SSCD_BASE_DIRECTORY_H_C43F1E0DA7CC438A8A7E90E43CFDCFEA
#include <QCoreApplication>

#ifdef Q_OS_WIN32
#define SSCD_BASE_DIRECTORY (QCoreApplication::instance()->applicationDirPath())
#else
#define SSCD_BASE_DIRECTORY QString("/usr/share/sscd")
#endif
#endif
