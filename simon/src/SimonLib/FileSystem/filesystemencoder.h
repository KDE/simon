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


#ifndef FILESYSTEMENCODER_H
#define FILESYSTEMENCODER_H

class QString;
/**
 \class FileSystemEncoder
 \author Peter Grasch
 \date 25.3. 2008
 \version 0.1

 Encodes Paths into "encoding-safe" equivalents (this can later be reverted).

 This prevents e.g. German "Umlaute" ("ä", "ü", etc.) from getting fucked up
*/
class FileSystemEncoder{
public:
    FileSystemEncoder() {}

	static QString encodeFilename(QString);
	static QString decodeFilename(QString);

    ~FileSystemEncoder(){}

};

#endif
