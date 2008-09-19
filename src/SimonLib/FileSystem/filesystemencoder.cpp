//
// C++ Implementation: filesystemencoder
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
