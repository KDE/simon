/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#include "filesystemencoder.h"
#include <QString>
/**
 * \brief Encode the given filename
 * \author Peter Grasch
 * @param  in filename
 * @return safe filename
 */
QString FileSystemEncoder::encodeFilename(QString in)
{
	in.replace("ü", "%ue");
	in.replace("ä", "%ue");
	in.replace("ö", "%oe");
	in.replace("Ü", "%Ue");
	in.replace("Ä", "%Ae");
	in.replace("Ö", "%Oe");
	in.replace("ß", "%Sz");
	return in;
}

/**
 * \brief Decode the given filename
 * \author Peter Grasch
 * @param  in Safe filename
 * @return Original filename
 */
QString FileSystemEncoder::decodeFilename(QString  in)
{
	in.replace("%ue", "ü");
	in.replace("%ae", "ä");
	in.replace("%oe", "ö");
	in.replace("%Ae", "Ä");
	in.replace("%Oe", "Ö");
	in.replace("%Ue", "Ü");
	in.replace("%Sz", "ß");
	return in;
}
