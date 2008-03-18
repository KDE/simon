/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include "addwordview.h"
#include <QWizardPage>
#include <QMessageBox>
#include <QLineEdit>
#include "logger.h"
#include "settings.h"
#include "addwordintropage.h"
#include "addwordrecordpage.h"
#include "addwordresolvepage.h"
#include "grammarmanager.h"
#include "wordlistmanager.h"
#include "modelmanager.h"


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
	promptsToAdd = new QHash<QString,QString>();
	this->welcomePage = createWelcomePage();
	resolvePage = createResolvePage();
	this->addPage((QWizardPage*) welcomePage);
	this->addPage(resolvePage);

	this->addPage(createRecordPage());
	this->addPage(createFinishedPage());
	
	connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));
	connect(ModelManager::getInstance(), SIGNAL(missingWord(QString)), this, SLOT(askToAddWord(QString)));
	connect(TrainingManager::getInstance(), SIGNAL(addMissingWords(QStringList)), this, SLOT(askToAddWords(QStringList)));

	setWindowTitle(tr("Wort hinzufügen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/addword.png"));
}


void AddWordView::cleanUp()
{
	wordsToAdd.clear();
	if (!listToAdd->isEmpty())
	{
		if (QMessageBox::question(this, tr("Bisherige Wörter hinzufügen"), tr("Wollen Sie die bisher fertig beschriebenen Wörter dieses Durchlaufes hinzufügen?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
			commitList();
		else 
		{
			listToAdd->clear();
			promptsToAdd->clear();
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
	finished->setTitle(tr("Hinzufügen des Wortes"));
	QLabel *label = new QLabel(finished);
	label->setText(tr("Es wurden alle benötigten Daten gesammelt.\n\nSimon kann das neue Wort jetzt lernen.\nBitte ueberprüfen Sie, bevor Sie hier\nbestätigen, ob die Aufnahmen nicht von\nHintergrundgeräuschen beeinträchtigt werden.\n\nKlicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschließen."));
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
	
	Logger::log(tr("[INF] Füge neues Wort zum Modell hinzu..."));
	Logger::log(tr("[INF] Neues Wort lautet: ")+word);
	
	listToAdd->append(Word(word, field("wordPronunciation").toString(),
		     field("wordTerminal").toString(), 2 /* 2 recordings */));
	promptsToAdd->insert(recordingName1, field("wordExample1").toString().toUpper());
	promptsToAdd->insert(recordingName2, field("wordExample2").toString().toUpper());
	
	if (wordsToAdd.count() > 0)
	{
		//multiple words
		createWord(wordsToAdd.takeAt(0));
	} else {
		commitList();
		restart();
	}


	//cleaning up
	Logger::log(tr("[INF] Wort hinzugefügt: ")+word);
	emit addedWord();
}


/**
 * \brief Adds the list of words to the wordlistmanager
 */
void AddWordView::commitList()
{
	//we can't know for certain if this will be sorted when we add multiple words at once
	WordListManager::getInstance()->addWords(listToAdd, false /*sorted*/, false /*shadowed*/);
	listToAdd = new WordList();
	// 	Training stuff deactivated for now
	TrainingManager::getInstance()->addSamples(promptsToAdd);
	promptsToAdd->clear();
}

void AddWordView::askToAddWord(QString word)
{
	if (QMessageBox::question(this, tr("Wort hinzufügen"), tr("Es wurde erkannt, dass das Wort \"%1\" im Sprachmodell fehlt.\n\nWollen Sie es jetzt hinzufügen?").arg(word), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
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

	if (QMessageBox::question ( 0, tr("Fehlende Wörter"), tr ( "Der zu trainierende Text enthält unbekannte Wörter. Diese sind:\n%1\n\nWollen Sie diese Wörter jetzt hinzufügen?").arg ( allWords ),QMessageBox::Yes|QMessageBox::No ) == QMessageBox::Yes)
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
		if (QMessageBox::question(this, tr("Wort hinzufügen Abbrechen?"), tr("Es wurde erkannt, dass gerade ein Wort hinzugefügt wird.\n\nWollen Sie das aktuelle Hinzufügen abbrechen?"), QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes)
			return;
	}
	restart();
	show();
	setField("wordNameIntro", word);
	next(); //continue to page 2
}
