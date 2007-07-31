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
	oldId = 0;
	this->addPage(createWelcomePage());
// 	this->addPage(createRecordPage());
	this->addPage(createFinishedPage());

	connect( this, SIGNAL(currentIdChanged( int )), this, 
			SLOT(pageChanged(int)));

// 	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(saveWord()));
	
// 	connect(this, SIGNAL(finished( int )), this, SLOT(finish()));

	setWindowTitle("Wort hinzufuegen");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/addword.png"));
}

void AddWordView::pageChanged(int id)
{
	if ((id==1) && (oldId == 0))
		saveWord();
	
	oldId = id;
}

QWizardPage* AddWordView::createWelcomePage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(tr("Hinzufuegen eines neuen Wortes"));
	QLabel *label = new QLabel(intro);
	label->setText("Mit Hilfe dieses Assistenten koennen Sie neue Woerter zum\nSprachmodell hinzufuegen. Geben Sie hierzu\nbitte den Namen des Wortes an\nund bestaetigen Sie mit \"Weiter\":\n\n");
	QLabel *lbName = new QLabel(intro);
	lbName->setText(tr("Neues Wort:"));
	QLineEdit *leName = new QLineEdit(intro);
	QVBoxLayout *layout = new QVBoxLayout(intro);
	QHBoxLayout *loName = new QHBoxLayout(intro);
	loName->addWidget(lbName);
	loName->addWidget(leName);

	layout->addWidget(label);
// 	intro->registerField("name*", leName));
	layout->addLayout(loName);
	
	intro->setLayout(layout);
	
	return intro;
}

QWizardPage* AddWordView::createRecordPage()
{
	
// 	rec1 = new RecWidget(tr("Aufnahme 1"), "1.wav", ui.wRec1);
// 	rec2 = new RecWidget(tr("Aufnahme 2"), "2.wav", ui.wRec2);
	
// 	ui.wRec1->layout()->addWidget(rec1);
// 	ui.wRec2->layout()->addWidget(rec2);
// 
// 	connect(rec1, SIGNAL(recordingFinished()), this, SLOT(checkReady()));
// 	connect(rec2, SIGNAL(recordingFinished()), this, SLOT(checkReady()));
// 	connect(rec1, SIGNAL(sampleDeleted()), this, SLOT(setNotReady()));
// 	connect(rec2, SIGNAL(sampleDeleted()), this, SLOT(setNotReady()));
}


QWizardPage* AddWordView::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle("Hinzufuegen des Wortes");
	QLabel *label = new QLabel(finished);
	label->setText("Es wurden alle benoetigten Daten gesammelt.\n\nSimon kann das neue Wort jetzt lernen.\nBitte ueberpruefen Sie, bevor Sie hier\nbestaetigen, ob die Aufnahmen nicht von\nHintergrundgeraeuschen beeintraechtigt werden.\n\nKlicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschlieszen.");
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

void AddWordView::checkReady()
{
// 	if (rec1->hasRecordingReady() && rec2->hasRecordingReady())
// 	{
// 		ui.pbNext_2->setEnabled(true);
// 	}
}

void AddWordView::setNotReady()
{
// 	ui.pbNext_2->setEnabled(false);
}

/**
 * \brief Saves the name of the word (input) in the member and sends the wizard to the next step
 * 
 * 
 * \author Peter Grasch
 */
void AddWordView::saveWord()
{
// 	this->word = ui.leWord->text();
	this->word = "test";
	
	Logger::log(tr("Fuege neues Wort zum Modell hinzu..."));
	Logger::log(tr("Neues Wort lautet: ")+this->word);
	
	this->setWindowTitle(tr("Wort hinzufuegen") + " - " + word);
}


/**
 * \brief Writes the word into the files and cleans up the wizard
 * 
 * \author Peter Grasch
 */
void AddWordView::finish()
{
	//finishs up
	
	//cleaning up
	Logger::log(tr("Word added: ")+this->word);
	
	rec1->deleteSample();
	rec2->deleteSample();
}


AddWordView::~AddWordView()
{
// 	delete rec1;
// 	delete rec2;
}
