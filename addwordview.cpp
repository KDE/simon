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
	
	rec = new WavRecorder();
	
	connect(ui.pbNext, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(prevStep()));
	connect(ui.pbNext_2, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(ui.pbBack_2, SIGNAL(clicked()), this, SLOT(prevStep()));
	
	connect(ui.pbRec1, SIGNAL(clicked()), this, SLOT(recSample1()));
	connect(ui.pbRec2, SIGNAL(clicked()), this, SLOT(recSample2()));
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
 * \brief Writes the word into the files and cleans up the wizard
 * 
 * \author Peter Grasch
 */
void AddWordView::finish()
{
	//finishs up
	
	//cleaning up
	QFile rec("1.wav");
	rec.remove();
	rec.setFileName("2.wav");
	rec.remove();
}

void AddWordView::recSample1Status(int msecs)
{
	ui.hsRec1->setMaximum(msecs/100);
	ui.lbRec1->setText("00:00 / "+QString::number(msecs/1000)+":"+QString::number(msecs/10-msecs/1000));
}

void AddWordView::recSample2Status(int msecs)
{
	ui.hsRec2->setMaximum(msecs/100);
	ui.lbRec2->setText("00:00 / "+QString::number(msecs/1000)+":"+QString::number(msecs/10-msecs/1000));
}


/**
 * \brief Starts the recording with the given Elements
 * 
 * \author Peter Grasch
 * 
 * \param QString filename
 * This is the filename to record to (e.g.: 1.wav)
 * \param QSlider *prog
 * Here the wavrecorder will store the current progress (in seconds) - this will later on be used as a position slider
 * \param QLabel *textprog
 * Here we will store the current length in the format "xx:xx / xx:xx"
 */
void AddWordView::startRecording(QString filename, QSlider *prog, QLabel *textprog)
{
	rec->record(filename, 2, 44100); // hardcoded stereo, 44100hz
	rec->start();
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
void AddWordView::stopRecording()
{
	rec->finish();
}

void AddWordView::recSample1()
{
	disconnect(ui.pbRec1, SIGNAL(clicked()), this, SLOT(recSample1()));
	connect (ui.pbRec1, SIGNAL(clicked()), this, SLOT(stopRecording()));
	this->startRecording("1.wav", ui.hsRec1, ui.lbRec1);
	disconnect(rec,0,0,0);
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(recSample1Status(int)));
}
void AddWordView::recSample2()
{
	this->startRecording("2.wav", ui.hsRec1, ui.lbRec1);
	disconnect(ui.pbRec2, SIGNAL(clicked()), this, SLOT(recSample2()));
	connect (ui.pbRec2, SIGNAL(clicked()), this, SLOT(stopRecording()));
	disconnect(rec,0,0,0);
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(recSample1Status(int)));
}

/**
 * @brief Sends the wizard to the previous step
 *
 * This function sends the wizard to the previous step.
 * There are three steps in this wizard.
 * 	* Spell the word
 * 	* Record two samples of the word
 * 	* Completion
 *
 * If there is no previous step it rejects the wizard
 *
 *	@author Peter Grasch
 * @see nextStep()
*/
void AddWordView::prevStep()
{
	if (ui.swMain->currentIndex() - 1 >= 0)
		ui.swMain->setCurrentIndex( ui.swMain->currentIndex()-1 );
	else reject();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
AddWordView::~AddWordView()
{

}

