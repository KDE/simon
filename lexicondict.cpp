//
// C++ Implementation: lexicondict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "lexicondict.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

LexiconDict::LexiconDict(QString path, QObject* parent): Dict(parent)
{
	this->path = path;
}


/**
 * \brief Loads the file from the given path
 * \author Peter Grasch
 * 
 * \todo Document
 * WARNING: This function assumes the system-charset to be ISO-8859-1 and WILL destroy special characters if it isn't
 * 
 * @param path If the path is empty the path set by the constructor is used
 */
void LexiconDict::load(QString path)
{
	if (path.isEmpty()) path = this->path;
	if (path.isEmpty()) return;

	QString unknownStr = tr("Unbekannt");
	emit progress(0);
	QFile *dict = new QFile(path);
	if (!dict->open(QIODevice::ReadOnly))
		return;
	QFileInfo info;
	info.setFile(path);
	int maxProg = info.size();
	int currentProg = 0;
	
	QTextStream *dictStream = new QTextStream(dict);
	dictStream->setCodec("ISO-8859-1");
	emit loaded();
	
	QString line, xsp;
	int wordstart, wordend;
	line = dictStream->readLine(1000);
	while (!line.isNull())
	{
		wordstart = line.indexOf("[");
		wordend = line.indexOf("]", wordstart+1);
		words << line.mid(wordstart+1,wordend-wordstart-1);
		terminals << unknownStr;
		pronunciations << line.mid(wordend+2).trimmed();
		
		currentProg += line.length();
		emit progress((int) (((((double)currentProg) / 
				((double)maxProg)))*1000));
		line = dictStream->readLine(1000);
	}
	dict->close();
}

LexiconDict::~LexiconDict()
{
}


