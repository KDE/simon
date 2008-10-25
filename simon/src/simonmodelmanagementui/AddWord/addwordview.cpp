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

#include <speechmodelmanagement/wordlistmanager.h>
#include <speechmodelmanagement/trainingmanager.h>

#include <QWizardPage>
#include <KMessageBox>
#include <KLineEdit>
#include <KStandardDirs>


AddWordView* AddWordView::instance;

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
	: QWizard (parent)
{
	prevId=0;
	listToAdd = new WordList();
	this->welcomePage = createWelcomePage();
	resolvePage = createResolvePage();
	this->addPage((QWizardPage*) welcomePage);
	this->addPage(resolvePage);

	this->addPage(createRecordPage());
	this->addPage(createFinishedPage());
	
	connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));
// 	connect(ModelManager::getInstance(), SIGNAL(missingWord(QString)), this, SLOT(askToAddWord(QString)));
	connect(TrainingManager::getInstance(), SIGNAL(addMissingWords(QStringList)), this, SLOT(askToAddWords(QStringList)));

	setWindowTitle(i18n("Wort hinzufügen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/addword.png")));
}


void AddWordView::cleanUp()
{
	wordsToAdd.clear();
	if (!listToAdd->isEmpty())
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("Wollen Sie die bisher fertig beschriebenen Wörter dieses Durchlaufes hinzufügen?")) == KMessageBox::Yes)
			commitList();
		else 
		{
			listToAdd->clear();
			promptsToAdd.clear();
		}
	}
}

AddWordView* AddWordView::getInstance()
{
	if (!instance)
		instance = new AddWordView(0);
	
	return instance;
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
QWizardPage* AddWordView::createRecordPage()
{
	AddWordRecordPage *add = new AddWordRecordPage(this);
	connect(add, SIGNAL(recordingNamesGenerated(QString,QString)), this, SLOT(setRecordingNames(QString, QString)));
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
	finished->setTitle(i18n("Hinzufügen des Wortes"));
	QLabel *label = new QLabel(finished);
	label->setWordWrap(true);
	label->setText(i18n("Es wurden alle benötigten Daten gesammelt.\n\nSimon kann das neue Wort jetzt lernen.\nBitte ueberprüfen Sie, bevor Sie hier bestätigen, ob die Aufnahmen nicht von Hintergrundgeräuschen beeinträchtigt werden.\n\nKlicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschließen."));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}




/**
 * \brief Writes the word into the files and cleans up the wizard
 * 
 * \author Peter Grasch
 */
void AddWordView::finish(int done)
{
	if (!done) return;
	
	QString word = field("wordName").toString();
	
	Logger::log(i18n("[INF] Füge neues Wort zum Modell hinzu..."));
	Logger::log(i18n("[INF] Neues Wort lautet: ")+word);
	
	listToAdd->append(Word(word, field("wordPronunciation").toString(),
		     field("wordTerminal").toString(), 2 /* 2 recordings */));
	promptsToAdd.insert(recordingName1, field("wordExample1").toString().toUpper());
	promptsToAdd.insert(recordingName2, field("wordExample2").toString().toUpper());
	
	if (wordsToAdd.count() > 0)
	{
		//multiple words
		createWord(wordsToAdd.takeAt(0));
	} else {
		commitList();
		restart();
	}


	//cleaning up
	Logger::log(i18n("[INF] Wort hinzugefügt: ")+word);
	emit addedWord();
}


/**
 * \brief Adds the list of words to the wordlistmanager
 */
void AddWordView::commitList()
{
	TrainingManager::getInstance()->addSamples(promptsToAdd, true /*recompiled later*/);
	promptsToAdd.clear();

	//we can't know for certain if this will be sorted when we add multiple words at once
	WordListManager::getInstance()->addWords(listToAdd, false /*sorted*/, false /*shadowed*/);
	listToAdd = new WordList();
}

void AddWordView::askToAddWord(QString word)
{
	if (KMessageBox::questionYesNoCancel(this, i18n("Es wurde erkannt, dass das Wort \"%1\" im Sprachmodell fehlt.\n\nWollen Sie es jetzt hinzufügen?", word)) == KMessageBox::Yes)
	{
		createWord(word);
	}
}

/**
 * \brief Asks the user if he wants to add the given list of words and will then start the wizard cycling over and over until all words are added
 * \author Susanne Tschernegg, Peter Grasch
 * @param words The words to add
 */
void AddWordView::askToAddWords(QStringList words)
{
	if (words.count() == 0) return;
	
	//tells the user, which words aren't in the dict
	QString allWords = words.join(", ");
// 	for ( int i=0; i<words.count(); i++ )
// 	{
// 		allWords += words.at ( i )+", ";
// 	}
// 	allWords = allWords.left(allWords.size()-2);

	if (KMessageBox::questionYesNoCancel ( 0, i18n ( "Der zu trainierende Text enthält unbekannte Wörter. Diese sind:\n%1\n\nWollen Sie diese Wörter jetzt hinzufügen?", allWords)) == KMessageBox::Yes)
	{
		wordsToAdd << words;
		createWord(wordsToAdd.takeAt(0));
	}
}

AddWordView::~AddWordView()
{
    resolvePage->deleteLater();
    welcomePage->deleteLater();
}

void AddWordView::setRecordingNames(QString name1, QString name2)
{
	this->recordingName1 = name1;
	this->recordingName2 = name2;
}

/**
 * \brief Shows the addWordView with a given word
 *          this method is used in the trainingmanager
 * \author Susanne Tschernegg, Peter Grasch
 */
void AddWordView::createWord(QString word)
{
	if (isVisible())
	{
		if (KMessageBox::questionYesNoCancel(this, i18n("Es wurde erkannt, dass gerade ein Wort hinzugefügt wird.\n\nWollen Sie das aktuelle Hinzufügen abbrechen?")) != KMessageBox::Yes)
			return;
	}
	restart();
	show();
	setField("wordNameIntro", word);
	next(); //continue to page 2
}
