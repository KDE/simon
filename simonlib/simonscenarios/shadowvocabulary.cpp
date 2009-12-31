/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#include "shadowvocabulary.h"
#include "trainingmanager.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <KColorScheme>
#include <KStandardDirs>
#include <KFilterDev>
#include <KMimeType>

/**
 * Empty, private constructor
 */
ShadowVocabulary::ShadowVocabulary() : Vocabulary(), loadFailed(false)
{
	kDebug() << "Initializing shadow dictionary: " << this;
	m_resolveProbability=false;
	QString vocabFilename = KStandardDirs::locate("appdata", "shadowvocabulary.xml");

	QIODevice *shadowVocabFile = KFilterDev::deviceForFile(vocabFilename,
						KMimeType::findByFileContent(vocabFilename)->name());

	reset(shadowVocabFile);
	delete shadowVocabFile;
}

bool ShadowVocabulary::reset(QIODevice* f)
{
	if (!f->open(QIODevice::ReadOnly)) {
		loadFailed = true;
		return false;
	}

	deleteAll();

	f->readLine(); //skip document type
	QString root = QString::fromUtf8(f->readLine()); //skip root element

	root = root.mid(26, 19);

	lastModifiedDate = QDateTime::fromString(root, Qt::ISODate);

	kDebug() << root << lastModifiedDate;

	while (!f->atEnd()) {
		//<word>
		//	<name>
		//		Aal
		//	</name>
		//	<pronunciation>
		//		a: l
		//	</pronunciation>
		//	<terminal>
		//		NOM
		//	</terminal>
		//</word>
		f->readLine(); //skip word
		f->readLine(); //skip name
		QString name = QString::fromUtf8(f->readLine()).trimmed();
		f->readLine(); //skip nameend

		f->readLine(); //skip pronunciation
		QString pronunciation = QString::fromUtf8(f->readLine()).trimmed();
		f->readLine(); //skip pronunciationend

		f->readLine(); //skip terminal
		QString terminal = QString::fromUtf8(f->readLine()).trimmed();
		f->readLine(); //skip terminalend
		f->readLine(); //skip wordend

		if (terminal.isEmpty()) continue;
		
		if (!terminals.contains(terminal)) terminals << terminal;

		m_words.append(new Word(name, pronunciation, terminal));
	}
	QAbstractItemModel::reset();
	loadFailed = false;
	return true;
}

int ShadowVocabulary::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 3;
}

void ShadowVocabulary::touch()
{
	lastModifiedDate = QDateTime::currentDateTime();
}

bool ShadowVocabulary::save()
{
	touch();

	QFile f(KStandardDirs::locateLocal("appdata", "shadowvocabulary.xml"));
	QIODevice *shadowVocabFile = KFilterDev::device(&f, "application/x-gzip", false);

	if (!shadowVocabFile->open(QIODevice::WriteOnly))
		return false;

	shadowVocabFile->write("<!DOCTYPE Vocabulary>\n");
	shadowVocabFile->write("<vocabulary lastModified=\""+lastModifiedDate.toString(Qt::ISODate).toUtf8()+"\">\n"); 

	foreach (Word *w, m_words) {
		//<word>
		//	<name>
		//		Aal
		//	</name>
		//	<pronunciation>
		//		a: l
		//	</pronunciation>
		//	<terminal>
		//		NOM
		//	</terminal>
		//</word>
		shadowVocabFile->write("<word>\n"); 
		shadowVocabFile->write("\t<name>\n"); 
		shadowVocabFile->write("\t\t"+w->getWord().toUtf8()+"\n");
		shadowVocabFile->write("\t</name>\n"); 

		shadowVocabFile->write("\t<pronunciation>\n");
		shadowVocabFile->write("\t\t"+w->getPronunciation().toUtf8()+"\n");
		shadowVocabFile->write("\t</pronunciation>\n"); 

		shadowVocabFile->write("\t<terminal>\n");
		shadowVocabFile->write("\t\t"+w->getTerminal().toUtf8()+"\n");
		shadowVocabFile->write("\t</terminal>\n"); 
		shadowVocabFile->write("</word>\n"); 
	}


	shadowVocabFile->write("</vocabulary>"); 
	shadowVocabFile->close();
	f.close();

	emit changed();

	return true;


/*	QDomDocument doc("vocabulary");

	QFile f(KStandardDirs::locateLocal("appdata", "shadowvocabulary.xml"));
	
	if (!f.open(QIODevice::WriteOnly))
		return false;

	QDomElement elem = serialize(&doc);
	doc.appendChild(elem);

	bool succ = f.write(doc.toString().toUtf8());

	if (succ)
		emit changed();

	return succ;*/
}

bool ShadowVocabulary::addWords(QList<Word*>* w)
{
	if (!Vocabulary::addWords(w)) return false;

	return save();
}

bool ShadowVocabulary::addWord(Word *w)
{
	if (!Vocabulary::addWord(w)) return false;
	return save();
}

bool ShadowVocabulary::removeWord(Word* w, bool deleteWord)
{
	if (Vocabulary::removeWord(w, deleteWord)) {
		return save();
	}
	return false;
}

bool ShadowVocabulary::renameTerminal(const QString& from, const QString& to)
{
	if (Vocabulary::renameTerminal(from, to))
		return save();
	return false;
}

