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


#include "grammarmanager.h"
#include <QFile>
#include <QCoreApplication>
#include <KLocalizedString>
#include <KLocale>
#include <QMutexLocker>
#include <QDateTime>
#include <simonlogging/logger.h>
#include "wordlistmanager.h"
#include "speechmodelmanagementconfiguration.h"



GrammarManager* GrammarManager::instance;

/**
 * \brief Constructor
 * \author Peter Grasch
 */
GrammarManager::GrammarManager() : QObject(), structuresLock(QMutex::Recursive)
{
	KLocale::setMainCatalog("simonlib");
	load();
}


GrammarManager * GrammarManager::getInstance()
{
	if (!instance)
		instance = new GrammarManager();
	return instance;
}

/**
 * \brief Loads the structures from the file
 * \author Peter Grasch
 * @return Successs
 */
bool GrammarManager::load()
{
	QMutexLocker lock(&structuresLock);

	structures.clear();
	
	QString path =KStandardDirs::locate("data", "simon/model/model.grammar");
	Logger::log(i18n("[INF] Loading grammar from %1", path));
	
	QFile grammar(path);
	if (!grammar.open(QIODevice::ReadOnly)) return false;
	
	QString structure;
	while (!grammar.atEnd()) 
	{
		structure = QString::fromUtf8(grammar.readLine(1500));
		structure.remove(0,7);		//remove the leading "S:NS_B "
		structure = structure.left(structure.count()-6);  //remove the trailing " NS_E"
		structures << structure;
	}
	
	grammar.close();

	return true;
}

/**
 * \brief Renames the terminal to the given, new name
 * @param terminal The terminal to renmae
 * @param newName The new name
 */
void GrammarManager::renameTerminal(QString terminal, const QString& newName)
{
	//make the terminal regex-able :)
	QMutexLocker lock(&structuresLock);

	terminal.replace(".", "\\.");
	terminal.replace("-", "\\-");
	terminal.replace("!", "\\!");
	terminal.replace("?", "\\?");
	terminal.replace("*", "\\*");
	terminal.replace("\\", "\\\\");
	terminal.replace("^", "\\^");
	terminal.replace("$", "\\$");

	//replace using regex patterns
	structures.replaceInStrings(QRegExp('^'+terminal+'$'), newName);
	structures.replaceInStrings(QRegExp(' '+terminal+'$'), ' '+newName);
	structures.replaceInStrings(QRegExp('^'+terminal+' '), newName+' ');
	structures.replaceInStrings(QRegExp(' '+terminal+' '), ' '+newName+' ');

	save();
// 	emit structuresChanged();
	//this turned out to be faster than the "per-hand" approach
	//...even if it looks a bit funny...
}

bool GrammarManager::refreshFiles(const QByteArray& grammarStructures)
{
	QMutexLocker lock(&structuresLock);
	QFile grammarF(KStandardDirs::locateLocal("data", "simon/model/model.grammar"));
	if (!grammarF.open(QIODevice::WriteOnly)) return false;
	grammarF.write(grammarStructures);
	grammarF.close();

	if (!load()) return false;
	emit structuresChanged();
	return true;
}

/**
 * \brief Get Examples for the word of the specified terminal
 * \author Peter Grasch
 * @param word The word to look for examples for
 * @param terminal The terminal of the word
 * @param count How many examples do we want?
 * @return Examples
 */
QStringList GrammarManager::getExamples(const QString& word, const QString& terminal, int count, bool includeShadow)
{
	QStringList grammarStructures = getStructures(terminal);

	if (grammarStructures.count()==0)
	{
		//couldn't resolve terminal - not in grammar definition
		return QStringList();
	}
	
	QStringList chosen;
	QString curTerminal;
	bool alreadyUsed=false;
	bool badWord=false;
	int chosenSentenceId;
	for (int i=0; (i < count) && (grammarStructures.count() > 0); i++)
	{
		chosenSentenceId = qrand()%grammarStructures.count();
		QString sentence = grammarStructures.at(chosenSentenceId);
	
		QStringList terminals = sentence.split(" ");
		
		badWord = false;
		//find random words for this terminals:
		//replace the first occurrance of the words terminal with the actual word we are trying to
		//"demonstrate in action"
		//this is ensured by the alreadyUsed variable which holds if the word was already replaced
		//if it is set the occurrance of the terminal is used just like any other
		for (int j=0; !badWord && (j < terminals.count()); j++)
		{
			
			curTerminal = terminals[j];
			if ((curTerminal == terminal) && (!alreadyUsed))
			{
				terminals.replace(j, word);
				alreadyUsed = true;
			} else
			{
				QString exampleWord =WordListManager::getInstance()->getRandomWord(terminals[j], includeShadow);
				if (exampleWord.isEmpty())
				{
					badWord = true;
				} else terminals.replace(j, exampleWord);
				
			}
		}
		if (badWord) 
		{
			grammarStructures.removeAt(chosenSentenceId);
			i--;
		}
		else
			chosen << terminals.join(" ").trimmed();
		alreadyUsed=false;
	}
	return chosen;
}

/**
 * \brief Returns all terminals contained in the grammar
 * \author Peter Grasch
 * @return The terminals
 */
QStringList GrammarManager::getTerminals()
{
	QMutexLocker lock(&structuresLock);
	QStringList out;
	QStringList terminalsInStruct;
// 	QStringList structures;// = CoreConfiguration::grammarStructures();
	for (int i=0; i < structures.count(); i++)
	{
		terminalsInStruct.clear();
		terminalsInStruct = structures[i].split(" ");
		for (int j=0; j < terminalsInStruct.count(); j++)
			if (!out.contains(terminalsInStruct[j]))
				out << terminalsInStruct[j];
	}
	return out;
}

/**
 * \brief Returns structures using the specified terminal
 * \author Peter Grasch
 * @param terminal The terminal to lok for
 * @return The structures
 */
QStringList GrammarManager::getStructures(const QString& terminal)
{
	QMutexLocker lock(&structuresLock);
	QStringList matching;
	int i=0;
// 	QStringList structures = getStructures();// = CoreConfiguration::grammarStructures();
	while (i < structures.count())
	{
		if(structures[i].contains(terminal)) matching << structures[i];
		i++;
	}
	return matching;
}

QStringList GrammarManager::getStructures()
{
	return structures;
}

void GrammarManager::setStructures(const QStringList &structures)
{
	this->structures = structures;
}

/**
 * \brief Saves the structures
 * \author Peter Grasch
 * @return success
 */
bool GrammarManager::save()
{
	QMutexLocker lock(&structuresLock);
	QString path =KStandardDirs::locateLocal("data", "simon/model/model.grammar");
	Logger::log(i18n("Loading grammar to %1", path));
	
	QFile grammar(path);
	if (!grammar.open(QIODevice::WriteOnly)) return false;
	
	for (int i=0; i < structures.count(); i++)
		grammar.write("S:NS_B "+structures[i].toUtf8()+" NS_E\n");
	
	grammar.close();

	
	KConfig config( KStandardDirs::locateLocal("data", "simon/model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("GrammarDate", QDateTime::currentDateTime());
	config.sync();

	emit structuresChanged();
	
	return true;
}
