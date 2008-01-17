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
#include "wordlistmanager.h"

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

AddWordView::AddWordView(QWidget *parent, WordListManager *wordlistMgr, TrainingManager *trainManager, GrammarManager *grammarManager)
	: QWizard (parent)
{
	prevId=0;
	this->wordlistMgr = wordlistMgr;
	this->trainManager = trainManager;
	this->welcomePage = createWelcomePage();
	resolvePage = createResolvePage(grammarManager);
	this->addPage((QWizardPage*) welcomePage);
	this->addPage(resolvePage);

	this->addPage(createRecordPage());
	this->addPage(createFinishedPage());
	
	connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));

	setWindowTitle(tr("Wort hinzufügen"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/banners/addword.png"));
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
AddWordResolvePage* AddWordView::createResolvePage(GrammarManager *grammarManager)
{
	return new AddWordResolvePage(wordlistMgr, grammarManager, this);
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
	//finishs up
	
	WordList *list = new WordList();

	list->append(Word(word, field("wordPronunciation").toString(),
			 field("wordTerminal").toString(), 2 /* 2 recordings */));

	QHash<QString,QString> samples;
	
	samples.insert(recordingName1, field("wordExample1").toString());
	samples.insert(recordingName2, field("wordExample2").toString());
	this->trainManager->addSamples(&samples);
	wordlistMgr->addWords(list, true /*sorted*/, false /*shadowed*/);

	//cleaning up
	Logger::log(tr("[INF] Wort hinzugefügt: ")+word);
	emit addedWord();
	
	((AddWordRecordPage*) this->page(2))->cleanUp();
	restart();
}

void AddWordView::setRecordingNames(QString name1, QString name2)
{
	this->recordingName1 = name1;
	this->recordingName2 = name2;
}

/**
 * \brief Shows the addWordView with a given word
 *          this method is used in the trainingmanager
 * \author Susanne Tschernegg
 */
void AddWordView::createWord(QString word)
{
//     welcomePage->setName(word);
	setField("wordNameIntro", word);
//     resolvePage->init(welcomePage->getName());
//     prevId = 1;
}
