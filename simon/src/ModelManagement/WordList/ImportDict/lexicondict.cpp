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


#include "lexicondict.h"
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QTextStream>
#include <KLocalizedString>
#include <KMimeType>
#include <KFilterDev>

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

	QString unknownStr = i18n("Unbekannt");

	emit progress(0);

	QIODevice *dict = KFilterDev::deviceForFile(path,
							KMimeType::findByFileContent(path)->name());
	if ((!dict) || (!dict->open(QIODevice::ReadOnly)))
		return;


	int maxProg=0;

	KMimeType::Ptr mimeType = KMimeType::findByFileContent(path);
	if (mimeType->is("text/plain")) //not compressed
	{
		QFileInfo info;
		info.setFile(QFile(path));
		maxProg = info.size();
	}

	int currentProg = 0;
	
	QTextStream *dictStream = new QTextStream(dict);
	dictStream->setCodec("ISO-8859-15");
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
		
		if (maxProg != 0)
			emit progress((int) (((((double)currentProg) / 
				((double)maxProg)))*1000));
				
		line = dictStream->readLine(1000);
	}
	dict->close();
    dict->deleteLater();
    delete dictStream;
}

LexiconDict::~LexiconDict()
{
}


