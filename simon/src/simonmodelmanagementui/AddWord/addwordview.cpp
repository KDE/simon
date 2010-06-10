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


#include "addwordview.h"
#include <simonlogging/logger.h>
#include "addwordintropage.h"
#include "addwordrecordpage.h"
#include <simonsound/trainsamplevolumepage.h>
#include "addwordresolvepage.h"
#include "../modelmanageruiproxy.h"

#include <simonsound/soundserver.h>
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/trainingmanager.h>
#include <simonscenarios/scenariomanager.h>

#include <QWizardPage>
#include <KMessageBox>
#include <KLineEdit>
#include <KStandardDirs>

bool isWordLessThan(Word *w1, Word *w2)
{
	if (w1->getLexiconWord() < w2->getLexiconWord())
		return true;
	else return ((w1->getLexiconWord() == w2->getLexiconWord()) && ((w1->getPronunciation() < w2->getPronunciation()) || 
						((w1->getPronunciation() == w2->getPronunciation()) && (w1->getTerminal() < w2->getTerminal()))));
}

/**
 * @brief Constructor
 *
 * Constructs a new AddWordView Object.
 * Initializes the UI and sets it to the first page
 *
 *	@author Peter Grasch
 * @param QWidget *parent
 * Parent of the Dialog; Default: 0
 * @param Qt::Wflags f
 * Qt Windowflags - default 0
*/

AddWordView::AddWordView(Vocabulary *vocab, QWidget *parent)
	: SimonWizard(parent),
	targetVocabulary(vocab),
	listToAdd(new QList<Word*>()),
	record1(createRecordPage("wordExample1", 1, 2)),
	record2(createRecordPage("wordExample2", 2, 2))
{
	this->addPage(createWelcomePage());
	this->addPage(createResolvePage());
	if (SoundServer::getCalibrateVolume())
		addPage(new TrainSampleVolumePage(this));
	this->addPage(record1);
	this->addPage(record2);
	this->addPage(createFinishedPage());


	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));

	setWindowTitle(i18n("Add Word"));
	setBanner("addword");
}



void AddWordView::accept()
{
	QString word = field("wordName").toString();
	
	Logger::log(i18n("[INF] Adding the new Word to the Model..."));
	Logger::log(i18n("[INF] New word: ")+word);

	listToAdd->append(new Word(word.trimmed(), field("wordPronunciation").toString(),
		     field("wordTerminal").toString()));
	
	QList<AddWordRecordPage*> recordPages;
	recordPages << record1;
	recordPages << record2;
	foreach (AddWordRecordPage *rec, recordPages)
	{
		//if rec doesn't have sample this list will be empty
		foreach (const QString& fileName, rec->getFileNames())
			promptsToAdd.insert(fileName, rec->getPrompt());

	}
	
	QStringList words = field("wordNameIntro").toString().split(" ", QString::SkipEmptyParts);
	if (words.count() > 0)
	{
		//multiple words
		record1->keepSample();
		record2->keepSample();
		show();
		advanceToResolvePage();
	} else {
		commitList();
		restart();
		QDialog::accept();
	}


	//cleaning up
	Logger::log(i18n("[INF] Added Word: ")+word);
	emit addedWord();
}


void AddWordView::cleanUp()
{
	setField("wordNameIntro", QString());
	if (!listToAdd->isEmpty())
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("Do you want to add the words that have been completely described up until now?")) == KMessageBox::Yes)
			commitList();
		else 
		{
			qDeleteAll(*listToAdd);
			listToAdd->clear();
			promptsToAdd.clear();
		}
	}
	record1->cleanUp();
	record2->cleanUp();
}


/**
 * \brief Creates the welcomepage
 * \author Peter Grasch
 * @return the QWizardPage
 */
AddWordIntroPage* AddWordView::createWelcomePage()
{
	return new AddWordIntroPage(this);
}

/**
 * \brief Creates the recordpage
 * \author Peter Grasch
 * @return the QWizardPage
 */
AddWordRecordPage* AddWordView::createRecordPage(const QString& fieldName, int pageNr, int pageMax)
{
	AddWordRecordPage *add = new AddWordRecordPage(fieldName, pageNr, pageMax);
	return add;
}

/**
 * \brief Creates the recordpage
 * \author Peter Grasch
 * @return the QWizardPage
 */
AddWordResolvePage* AddWordView::createResolvePage()
{
	return new AddWordResolvePage(this);
}


/**
 * \brief Creates the finishedpage
 * \author Peter Grasch
 * @return the QWizardPage
 */
QWizardPage* AddWordView::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(i18n("Word Added"));
	QLabel *label = new QLabel(finished);
	label->setWordWrap(true);
	label->setText(i18n("The needed data has been collected.\n\nSimon can now \"learn\" the new word."));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}



/**
 * \brief Adds the list of words to the wordlistmanager
 */
void AddWordView::commitList()
{
	ModelManagerUiProxy::getInstance()->startGroup();
	foreach (const QString& key, promptsToAdd.keys())
	{
		if (!TrainingManager::getInstance()->addSample(key, promptsToAdd.value(key)))
			KMessageBox::error(this, i18n("Couldn't add %1 to the Prompts-Table", key));
	}
	if (!TrainingManager::getInstance()->savePrompts())
		KMessageBox::error(this, i18n("Couldn't save prompts"));
		
	promptsToAdd.clear();

	for (int i=0; i < listToAdd->count(); i++)
	{
		Word *w = listToAdd->takeAt(i);
		listToAdd->insert(i, w);
	}

	//sort the list
	qSort(listToAdd->begin(), listToAdd->end(), isWordLessThan);

	bool success=true;
	if (!targetVocabulary) {
		Scenario *s = ScenarioManager::getInstance()->getCurrentScenario();
		if (s)	
			success = s->addWords(listToAdd);
		else success = false;
	} else success = targetVocabulary->addWords(listToAdd);
	
	if (!success)
		KMessageBox::sorry(this, "Couldn't add word(s).");

	listToAdd = new QList<Word*>();
	ModelManagerUiProxy::getInstance()->commitGroup();
}

void AddWordView::askToAddWord(QString word)
{
	if (KMessageBox::questionYesNoCancel(this, i18n("The word \"%1\" appears to be missing in your language model.\n\nDo you want to add it now?", word)) == KMessageBox::Yes)
	{
		createWord(word);
	}
}

/**
 * \brief Asks the user if he wants to add the given list of words and will then start the wizard cycling over and over until all words are added
 * \author Susanne Tschernegg, Peter Grasch
 * @param words The words to add
 */
void AddWordView::addWords(QStringList words)
{
	if (words.count() == 0) return;
	
	createWord(words.join(" "));
}

void AddWordView::advanceToResolvePage()
{
	QString words = field("wordNameIntro").toString();
	restart();
	setField("wordNameIntro", words);
	next();
}

/**
 * \brief Shows the addWordView with a given word
 *          this method is used in the trainingmanager
 * \author Susanne Tschernegg, Peter Grasch
 */
void AddWordView::createWord(QString word)
{
	setField("wordNameIntro", word);
	advanceToResolvePage();
}


AddWordView::~AddWordView()
{
}

