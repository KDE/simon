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

#include "modelmanageruiproxy.h"
#include "AddWord/addwordview.h"
#include <KMessageBox>
#include <KDebug>
#include <KStandardDirs>
#include <KLocalizedString>
#ifdef SIMON_SCENARIOS
#include <speechmodelmanagement_scenario/trainingmanager.h>
#else
#include <speechmodelmanagement/trainingmanager.h>
#endif


ModelManagerUiProxy::ModelManagerUiProxy(QObject *parent) : ModelManager(parent)
{
	connect (this, SIGNAL(modelChanged()), this, SLOT(slotModelChanged()));

	if (ModelManager::instance)
		ModelManager::instance->deleteLater();
	
	ModelManager::instance = this;
}


void ModelManagerUiProxy::slotModelChanged()
{
	emit recompileModel();
}

//TODO: is this deprecated?
void ModelManagerUiProxy::displayCompilationProtocol(const QString& protocol)
{
	KMessageBox::detailedSorry(0, i18n("Protocol:"), protocol);
}

void ModelManagerUiProxy::sampleNotAvailable(const QString& sample)
{
	if (KMessageBox::questionYesNo(0, i18n("The sample \"%1\" could not be found neither on the local computer nor the "
"server.\n\nDo you want to remove it from the Trainings-Database?", sample)) == KMessageBox::Yes)
	{
		//kick some poor samples ass
		startGroup();
		QString sampleBaseName = sample.left(sample.length()-4);
		kDebug() << "Deleting: " << sampleBaseName;
		bool succ = TrainingManager::getInstance()->removeSample(sampleBaseName);
		if (succ)
			TrainingManager::getInstance()->savePrompts();

		commitGroup(false /*silent*/);

		if (!succ)
			KMessageBox::error(0, i18n("Couldn't remove Sample from the Trainingscorpus"));
	}
}




void ModelManagerUiProxy::wordUndefined(const QString& word)
{
	if (KMessageBox::questionYesNoCancel(0, i18n("The word \"%1\" is used in your training-samples but is not contained "
"in your wordlist.\n\nDo you want to add the word now?", word)) != KMessageBox::Yes)
		return;
//	KMessageBox::information(0, i18n("Sorry this is not yet implemented"));

	AddWordView *addWordView = new AddWordView(0);
	addWordView->createWord(word);
	addWordView->show();
	connect(addWordView, SIGNAL(finished(int)), addWordView, SLOT(deleteLater()));
}

void ModelManagerUiProxy::classUndefined(const QString& undefClass)
{
	KMessageBox::sorry(0, i18n("Your grammar uses the undefined terminal \"%1\".\n\nPlease add a word that uses this terminal or remove the structure(s) containing the terminal from your grammar.", undefClass));
}

void ModelManagerUiProxy::phonemeUndefined(const QString& phoneme)
{
	KMessageBox::sorry(0, i18n("The Phoneme \"%1\" is undefined.\n\nPlease train at least one word that uses it.", phoneme));
}


bool ModelManagerUiProxy::storeWordList(const QDateTime& changedTime, const QByteArray& simpleVocab,
			const QByteArray& activeVocab, const QByteArray& activeLexicon)
{
	bool succ = ModelManager::storeWordList(changedTime, simpleVocab, activeVocab, activeLexicon);
	if (!succ) {
		KMessageBox::sorry(0, i18n("Could not store the wordlist received from the server."
					"\n\nPlease check the permissions on the model folder: %1", 
					KStandardDirs::locateLocal("appdata", "model")));
	}
	return succ;
}

bool ModelManagerUiProxy::storeGrammar(const QDateTime& changedTime, const QByteArray& grammarStructures)
{
	bool succ = ModelManager::storeGrammar(changedTime, grammarStructures);
	if (!succ) {
		KMessageBox::sorry(0, i18n("Could not store the grammar received from the server."
					"\n\nPlease check the permissions on the model folder: %1", 
					KStandardDirs::locateLocal("appdata", "model")));
	}
	return succ;
}

bool ModelManagerUiProxy::storeLanguageDescription(const QDateTime& changedTime, const QByteArray& shadowVocab, 
			const QByteArray& treeHed)
{
	bool succ = ModelManager::storeLanguageDescription(changedTime, shadowVocab, treeHed);
	if (!succ) {
		KMessageBox::sorry(0, i18n("Could not store the language description received from the server."
					"\n\nPlease check the permissions on the model folder: %1", 
					KStandardDirs::locateLocal("appdata", "model")));
	}
	return succ;
}

bool ModelManagerUiProxy::storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
			const QByteArray& prompts)
{
	bool succ = ModelManager::storeTraining(changedTime, sampleRate, wavConfig, prompts);
	if (!succ) {
		KMessageBox::sorry(0, i18n("Could not store the trainings corpus received from the server."
					"\n\nPlease check the permissions on the model folder: %1", 
					KStandardDirs::locateLocal("appdata", "model")));
	}
	return succ;
}

bool ModelManagerUiProxy::storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& hmmDefs,
		const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa)
{
	bool succ = ModelManager::storeActiveModel(changedTime, sampleRate, hmmDefs, tiedList, dict, dfa);
	if (!succ) {
		KMessageBox::sorry(0, i18n("Could not store the active model received from the server."
					"\n\nPlease check the permissions on the model folder: %1", 
					KStandardDirs::locateLocal("appdata", "model")));
	}
	return succ;
}

bool ModelManagerUiProxy::storeSample(const QByteArray& sample)
{
	bool succ = ModelManager::storeSample(sample);
	if (!succ) {
		KMessageBox::sorry(0, i18n("Could not store the sample %1 received from the server."
					"\n\nPlease check the permissions on the sample folder: %2", 
					missingSample(), TrainingManager::getInstance()->getTrainingDir()));
	}
	return succ;
}





ModelManagerUiProxy::~ModelManagerUiProxy()
{

}

