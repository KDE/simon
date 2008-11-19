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
	listToAdd = new WordList();
	this->addPage(createWelcomePage());
	this->addPage(createResolvePage());

	record1 = createRecordPage("wordExample1", 1, 2);
	record2 = createRecordPage("wordExample2", 2, 2);
	
	this->addPage(record1);
	this->addPage(record2);
	
	this->addPage(createFinishedPage());
	
	connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));
	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));

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
	finished->setTitle(i18n("Hinzufügen Abgeschlossen"));
	QLabel *label = new QLabel(finished);
	label->setWordWrap(true);
	label->setText(i18n("Es wurden alle benötigten Daten gesammelt.\n\nSimon kann das neue Wort jetzt lernen.\n\nDer Assistent ist hiermit abgeschlossen."));
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
	promptsToAdd.insert(record1->getFileName(), record1->getPrompt().toUpper());
	promptsToAdd.insert(record2->getFileName(), record2->getPrompt().toUpper());
	
	if (wordsToAdd.count() > 0)
	{
		//multiple words
		createWord(wordsToAdd.takeAt(0));
		show();
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
	foreach (const QString& key, promptsToAdd.keys())
	{
		if (!TrainingManager::getInstance()->addSample(key, promptsToAdd.value(key)))
		{
			KMessageBox::error(this, i18n("Konnte %1 nicht zur Promptstable hinzufügen", key));
		}
	}
	if (!TrainingManager::getInstance()->savePrompts())
		KMessageBox::error(this, i18n("Konnte Prompts nicht speichern"));
		
	promptsToAdd.clear();

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
void AddWordView::addWords(QStringList words)
{
	if (words.count() == 0) return;
	
	wordsToAdd << words;
	createWord(wordsToAdd.takeAt(0));
}

AddWordView::~AddWordView()
{
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
	setField("wordNameIntro", word);
	next(); //continue to page 2
}
