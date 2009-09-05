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
#include "addwordresolvepage.h"
#include "../modelmanageruiproxy.h"

#ifdef SIMON_SCENARIOS
#include <speechmodelmanagement_scenario/wordlistmanager.h>
#include <speechmodelmanagement_scenario/trainingmanager.h>
#else
#include <speechmodelmanagement/wordlistmanager.h>
#include <speechmodelmanagement/trainingmanager.h>
#endif

#include <QWizardPage>
#include <KMessageBox>
#include <KLineEdit>
#include <KStandardDirs>


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

AddWordView::AddWordView(QWidget *parent)
	: QWizard(parent),
	listToAdd(new WordList()),
	record1(createRecordPage("wordExample1", 1, 2)),
	record2(createRecordPage("wordExample2", 2, 2))
{
	this->addPage(createWelcomePage());
	this->addPage(createResolvePage());
	this->addPage(record1);
	this->addPage(record2);
	this->addPage(createFinishedPage());
	
	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));

	setWindowTitle(i18n("Add Word"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/addword.png")));
}



void AddWordView::accept()
{
	QString word = field("wordName").toString();
	
	Logger::log(i18n("[INF] Adding the new Word to the Model..."));
	Logger::log(i18n("[INF] New word: ")+word);
	
	int recordingCount=0;
	if (record1->hasSample()) {
		promptsToAdd.insert(record1->getFileName(), record1->getPrompt().toUpper());
		recordingCount++;
	}
	if (record2->hasSample()) {
		promptsToAdd.insert(record2->getFileName(), record2->getPrompt().toUpper());
		recordingCount++;
	}
#ifdef SIMON_SCENARIOS
	listToAdd->append(Word("scenarioNotSet", word, field("wordPronunciation").toString(),
		     field("wordTerminal").toString(), recordingCount));
#else
	listToAdd->append(Word(word, field("wordPronunciation").toString(),
		     field("wordTerminal").toString(), recordingCount));
#endif
	
	if (wordsToAdd.count() > 0)
	{
		//multiple words
		createWord(wordsToAdd.takeAt(0));
		record1->keepSample();
		record2->keepSample();
		show();
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
	wordsToAdd.clear();
	if (!listToAdd->isEmpty())
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("Do you want to add the words that have been completely described up until now?")) == KMessageBox::Yes)
			commitList();
		else 
		{
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
		{
			KMessageBox::error(this, i18n("Couldn't add %1 to the Prompts-Table", key));
		}
	}
	if (!TrainingManager::getInstance()->savePrompts())
		KMessageBox::error(this, i18n("Couldn't save prompts"));
		
	promptsToAdd.clear();

	for (int i=0; i < listToAdd->count(); i++)
	{
		Word w = listToAdd->takeAt(i);
		w.setProbability(TrainingManager::getInstance()->getProbability(w.getWord()));
		listToAdd->insert(i, w);
	}

	WordListManager::getInstance()->addWords(listToAdd, false /*sorted*/, false /*shadowed*/);
	listToAdd = new WordList();
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
	
	wordsToAdd << words;
	createWord(wordsToAdd.takeAt(0));
}

/**
 * \brief Shows the addWordView with a given word
 *          this method is used in the trainingmanager
 * \author Susanne Tschernegg, Peter Grasch
 */
void AddWordView::createWord(QString word)
{
	restart();
	setField("wordNameIntro", word);
	next(); //continue to page 2
}

AddWordView::~AddWordView()
{
}

