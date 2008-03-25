//
// C++ Interface: filesystemencoder
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
