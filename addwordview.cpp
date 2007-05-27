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

AddWordView::AddWordView(QWidget *parent, Qt::WFlags f)
	: QDialog (parent, f)
{
	ui.setupUi(this);
	
	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(saveWord()));
	
	connect(ui.pbNext, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(prevStep()));
	connect(ui.pbNext_2, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(ui.pbBack_2, SIGNAL(clicked()), this, SLOT(prevStep()));
	connect(ui.pbBack_3, SIGNAL(clicked()), this, SLOT(prevStep()));
	connect(ui.pbFinish, SIGNAL(clicked()), this, SLOT(finish()));
	
	rec1 = new RecWidget("Aufnahme 1", "1.wav", ui.wRec1);
	rec2 = new RecWidget("Aufnahme 2", "2.wav", ui.wRec2);
	
	ui.wRec1->layout()->addWidget(rec1);
	ui.wRec2->layout()->addWidget(rec2);
	
	connect(rec1, SIGNAL(recordingFinished()), this, SLOT(checkReady()));
	connect(rec2, SIGNAL(recordingFinished()), this, SLOT(checkReady()));
	connect(rec1, SIGNAL(sampleDeleted()), this, SLOT(setNotReady()));
	connect(rec2, SIGNAL(sampleDeleted()), this, SLOT(setNotReady()));
}

void AddWordView::checkReady()
{
	if (rec1->hasRecordingReady() && rec2->hasRecordingReady())
	{
		ui.pbNext_2->setEnabled(true);
	}
}

void AddWordView::setNotReady()
{
	ui.pbNext_2->setEnabled(false);
}

/**
 * \brief Saves the name of the word (input) in the member and sends the wizard to the next step
 * 
 * 
 * \author Peter Grasch
 */
void AddWordView::saveWord()
{
	this->word = ui.leWord->text();
	this->setWindowTitle(tr("Wort hinzufügen") + " - " + word);
}


/**
 * @brief Sends the wizard to the next step
 *
 * This function sends the wizard to the next step.
 * There are three steps in this wizard.
 * 	* Spell the word
 * 	* Record two samples of the word
 * 	* Completion
 *
 * 
 * If there is no step left, it calls the method finish()
 *
 *	@author Peter Grasch
 * @see prevStep() finish()
 */
void AddWordView::nextStep()
{
	if (ui.swMain->currentIndex() + 1 < ui.swMain->count())
		ui.swMain->setCurrentIndex( ui.swMain->currentIndex()+1 );
	else finish();
}

/**
 * @brief Sends the wizard to the previous step
 *
 * This function sends the wizard to the next step.
 * There are three steps in this wizard.
 * 	* Spell the word
 * 	* Record two samples of the word
 * 	* Completion
 *
 * 
 * If there is no step left, it calls the method finish()
 *
 *	@author Peter Grasch
 * @see nextStep() finish()
 */
void AddWordView::prevStep()
{
	if (ui.swMain->currentIndex() - 1 >= 0)
		ui.swMain->setCurrentIndex( ui.swMain->currentIndex()-1 );
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
	
	//close the dialog
	accept();
	
	//ui clean up
	ui.swMain->setCurrentIndex(0);
	ui.leWord->setText("");
	setWindowTitle(tr("Wort hinzufügen"));
	
	rec1->deleteSample();
	rec2->deleteSample();
}


AddWordView::~AddWordView()
{
	delete rec1;
	delete rec2;
}