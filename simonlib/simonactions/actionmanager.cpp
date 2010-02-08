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

#include "actionmanager.h"

#include "listcommand.h"
#include "commandsettings.h"

#include <simoninfo/simoninfo.h>
//#include <simonscenarios/commandconfiguration.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/commandmanager.h>
#include <simonscenarios/createcommandwidget.h>

#include <QFile>
#include <QMetaObject>

#include <KMessageBox>
#include <KLocalizedString>
#include <KService>
#include <KServiceTypeTrader>
#include <KStandardDirs>
#include <KDesktopFile>
#include <KDebug>
#include <KLocale>
#include <KXmlGuiWindow>
#include <KXMLGUIClient>
#include <KXMLGUIFactory>



ActionManager* ActionManager::instance;


ActionManager::ActionManager(QObject* parent) : QObject(parent),
	currentlyPromptedListOfResults(0),
	greedyReceivers(new QList<GreedyReceiver*>()),
	minimumConfidenceThreshold(0.7),
	useDYM(true)
{
	KLocale::setMainCatalog("simonlib");
	retrieveRecognitionResultFilteringParameters();
}

void ActionManager::registerGreedyReceiver(GreedyReceiver *receiver)
{
	greedyReceivers->insert(0, receiver);
}

void ActionManager::deRegisterGreedyReceiver(GreedyReceiver *receiver)
{
	greedyReceivers->removeAll(receiver);
}

void ActionManager::retrieveRecognitionResultFilteringParameters()
{
	useDYM = CommandSettings::getInstance()->useDYM();
	minimumConfidenceThreshold = CommandSettings::getInstance()->minimumConfidence();
}

bool ActionManager::triggerCommand(const QString& type, const QString& trigger)
{
	if (type == "simonrecognitionresult" && currentlyPromptedListOfResults) {
		//result from a did-you-mean popup
		QString selectedSentence = trigger;
		selectedSentence.remove(QRegExp("^[0-9][0-9]?[0-9]?%: "));
		for (int i=0; i< currentlyPromptedListOfResults->count(); i++)
		{
			QString sentence = currentlyPromptedListOfResults->at(i).sentence();
			if (sentence == selectedSentence) {
				kDebug() << "Found the result!";
				RecognitionResultList *list = new RecognitionResultList();
				list->append(RecognitionResult(currentlyPromptedListOfResults->at(i)));
				delete currentlyPromptedListOfResults;
				currentlyPromptedListOfResults = 0;
				processRawResults(list);
				return true;
			}
		}
		return true;
	}

	return ScenarioManager::getInstance()->triggerCommand(type, trigger);
}

bool ActionManager::runLauncher(const QString& trigger)
{
	return processResult(RecognitionResult(trigger, "", "", QList<float>() << 1));
}

bool ActionManager::processResult(RecognitionResult recognitionResult)
{
	if (!greedyReceivers->isEmpty()) {
		bool accepted = false;
		for (int i=0; i < greedyReceivers->count(); i++) {
			if (greedyReceivers->at(i)->greedyTriggerRaw(recognitionResult))
				accepted = true;
				break;
		}
		return accepted;
	}

	return ScenarioManager::getInstance()->processResult(recognitionResult);
}

void ActionManager::processRawResults(RecognitionResultList* recognitionResults)
{
	if (recognitionResults->isEmpty())
		return;

	kDebug() << "Processing " << recognitionResults->count() << " raw results";

	RecognitionResultList *selectedRecognitionResults = new RecognitionResultList();

	if (!currentlyPromptedListOfResults || currentlyPromptedListOfResults->isEmpty()) {
		for (int i=0; i < recognitionResults->count(); i++) {
		//foreach (const RecognitionResult& result, recognitionResults) {
			//if the recognition result has:
			//	* One word that has a score of 0
			//	* An average score of below the minimum confidence
			//it will be not be included in the list of results
			
			QList<float> confidenceScores = recognitionResults->at(i).confidenceScores();
			kDebug() << confidenceScores;

			//calc average
			float avg= recognitionResults->at(i).averageConfidenceScore();
			kDebug() << avg << minimumConfidenceThreshold;

			if (!confidenceScores.contains(0.0f) && (avg >= minimumConfidenceThreshold))
				selectedRecognitionResults->append(recognitionResults->at(i));
		}

		kDebug() << "Viable recognition results: " << selectedRecognitionResults->count();

		if (selectedRecognitionResults->count() == 0) return;
	} else {
		//we are already asking...
		selectedRecognitionResults->append(recognitionResults->at(0));
	}

	kDebug() << "Greedy Recievers: " << greedyReceivers->count();

	if (!greedyReceivers->isEmpty()) {
		for (int i=0; i < greedyReceivers->count(); i++) {
			if (greedyReceivers->at(i)->greedyTriggerRawList(selectedRecognitionResults))
				break;
		}
		delete selectedRecognitionResults;
		return;
	}

	if (selectedRecognitionResults->count() == 1) {
		processResult(selectedRecognitionResults->at(0));
	} else {
		presentUserWithResults(selectedRecognitionResults);
	}
	delete selectedRecognitionResults;
}

CommandList* ActionManager::getCommandList()
{
	return ScenarioManager::getInstance()->getCommandList();
}

void ActionManager::resultSelectionDone()
{
	kDebug() << "resultSelectionDone()";
	delete currentlyPromptedListOfResults;
	currentlyPromptedListOfResults = 0;
}

void ActionManager::presentUserWithResults(RecognitionResultList* recognitionResults)
{
	kDebug() << "More than one possible recognition result ... should display list!";
	if (!useDYM || (currentlyPromptedListOfResults && 
				!currentlyPromptedListOfResults->isEmpty()))
	{
		//no double did-you-means...
		processResult(recognitionResults->at(0));
		return;
	}

	delete currentlyPromptedListOfResults;
	currentlyPromptedListOfResults = new RecognitionResultList();

	QStringList sentences;
	QStringList trigger;
	QStringList iconSrcs;
	for (int i=0; i<recognitionResults->count(); i++) {

		QList<float> confidenceScores = recognitionResults->at(i).confidenceScores();

		float avg = recognitionResults->at(i).averageConfidenceScore();
		avg *= 100;


		sentences << QString("%1%: %2").arg(qRound(avg)/*, 0, 'f', 0*/).arg(recognitionResults->at(i).sentence());
		iconSrcs << "";
		trigger << "simonrecognitionresult";
		currentlyPromptedListOfResults->append(recognitionResults->at(i));

	}

	ListCommand *list = new ListCommand(i18n("Did you mean ...?"), "help-hint", i18n("simon is not shure what you meant.\n\nPlease select the correct result from the list below."), sentences, iconSrcs, trigger);
	connect(list, SIGNAL(canceled()), list, SLOT(deleteLater()));
	connect(list, SIGNAL(canceled()), this, SLOT(resultSelectionDone()));
	connect(list, SIGNAL(entrySelected()), list, SLOT(deleteLater()));
	int state = SimonCommand::DefaultState;
	list->trigger(&state);
}

QFont ActionManager::pluginBaseFont()
{
	return CommandSettings::getInstance()->pluginBaseFont();
}

QList<CommandLauncher*> ActionManager::getLauncherList()
{
	return ScenarioManager::getInstance()->getLauncherList();
}


ActionManager::~ActionManager()
{
	//...
}
