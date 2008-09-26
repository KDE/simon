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
#include <KMessageBox>
#include <KLocalizedString>
#include "../../SimonLib/Logging/logger.h"
#include "../WordList/wordlistmanager.h"
#include "../modelmanager.h"
#include "coreconfiguration.h"



GrammarManager* GrammarManager::instance;

/**
 * \brief Constructor
 * \author Peter Grasch
 */
GrammarManager::GrammarManager() : QObject()
{
	connect(ModelManager::getInstance(), SIGNAL(unknownGrammarClass(QString)), this, SLOT(unknownWordClass(QString)));
// 	load();
}


void GrammarManager::unknownWordClass(QString name)
{
	KMessageBox::error(0, i18n("Der Terminal \"%1\" kommt in Ihrere Grammatikdefinition vor, nicht aber in der Wortliste.\n\nDies ist nicht gültig.\n\nBitte fügen Sie entweder ein Wort dieses Terminals zu Ihrem aktiven Wortschatz hinzu oder löschen Sie das betreffende Satzkonstrukt.", name), i18n("Nicht benutzte Wortklasse"));
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
// bool GrammarManager::load()
// {
// 	structures.clear();
// 	
// 	QString path =Settings::getS("Model/PathToGrammar");
// 	Logger::log(i18n("[INF] Lade Grammatik von %1", path));
// 	
// 	QFile grammar(Settings::getS("Model/PathToGrammar"));
// 	if (!grammar.open(QIODevice::ReadOnly)) return false;
// 	
// 	QString structure;
// 	while (!grammar.atEnd()) 
// 	{
// 		structure = QString::fromUtf8(grammar.readLine(1500));
// 		structure.remove(0,7);		//remove the leading "S:NS_B "
// 		structure = structure.left(structure.count()-6);  //remove the trailing " NS_E"
// 		structures << structure;
// 	}
// 	
// 	grammar.close();
// 	
// 	return true;
// }

/**
 * \brief Renames the terminal to the given, new name
 * @param terminal The terminal to renmae
 * @param newName The new name
 */
void GrammarManager::renameTerminal(QString terminal, QString newName)
{
	//make the terminal regex-able :)
	terminal.replace(".", "\\.");
	terminal.replace("-", "\\-");
	terminal.replace("!", "\\!");
	terminal.replace("?", "\\?");
	terminal.replace("*", "\\*");
	terminal.replace("\\", "\\\\");
	terminal.replace("^", "\\^");
	terminal.replace("$", "\\$");

	//replace using regex patterns
	QStringList structures = CoreConfiguration::grammarStructures();
	structures.replaceInStrings(QRegExp('^'+terminal+'$'), newName);
	structures.replaceInStrings(QRegExp(' '+terminal+'$'), ' '+newName);
	structures.replaceInStrings(QRegExp('^'+terminal+' '), newName+' ');
	structures.replaceInStrings(QRegExp(' '+terminal+' '), ' '+newName+' ');
	CoreConfiguration::setGrammarStructures(structures);

	//this turned out to be faster than the "per-hand" approach
	//...even if it looks a bit funny...
}

/**
 * \brief Get Examples for the word of the specified terminal
 * \author Peter Grasch
 * @param word The word to look for examples for
 * @param terminal The terminal of the word
 * @param count How many examples do we want?
 * @return Examples
 */
QStringList GrammarManager::getExamples(QString word, QString terminal, int count, bool includeShadow)
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
	QStringList out;
	QStringList terminalsInStruct;
	QStringList structures = CoreConfiguration::grammarStructures();
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
QStringList GrammarManager::getStructures(QString terminal)
{
	QStringList matching;
	int i=0;
	QStringList structures = CoreConfiguration::grammarStructures();
	while (i < structures.count())
	{
		if(structures[i].contains(terminal)) matching << structures[i];
		i++;
	}
	return matching;
}

/**
 * \brief Saves the structures
 * \author Peter Grasch
 * @return success
 */
// bool GrammarManager::save()
// {
// 	QString path =Settings::getS("Model/PathToGrammar");
// 	Logger::log(i18n("[INF] Speichere Grammatik nach %1", path));
// 	
// 	QFile grammar(Settings::getS("Model/PathToGrammar"));
// 	if (!grammar.open(QIODevice::WriteOnly)) return false;
// 	
// 	for (int i=0; i < structures.count(); i++)
// 		grammar.write("S:NS_B "+structures[i].toUtf8()+" NS_E\n");
// 	
// 	grammar.close();
// 	
// 	return true;
// }
