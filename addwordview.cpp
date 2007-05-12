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
	
	connect(ui.pbRec1, SIGNAL(clicked()), this, SLOT(recSample1()));
	connect(ui.pbRec2, SIGNAL(clicked()), this, SLOT(recSample2()));
	
	connect (ui.pbPlay1, SIGNAL(clicked()), this, SLOT(playSample1()));
	connect (ui.pbPlay2, SIGNAL(clicked()), this, SLOT(playSample2()));
	
	connect(ui.pbStartOver1, SIGNAL(clicked()), this, SLOT(deleteSample1()));
	connect(ui.pbStartOver2, SIGNAL(clicked()), this, SLOT(deleteSample2()));
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
	deleteSample1();
	deleteSample2();
}

/**
 * \brief Makes the progress information for the 1st sample reflect the current progress
 * 
 * Sets the label and the slider maxes to the given msecs
 * 
 * \author Peter Grasch
 * \param int msecs
 * The progress
 */
void AddWordView::recSample1Status(int msecs)
{
	setRecStatus(ui.hsRec1, ui.lbRec1, msecs);
}

/**
 * \brief Makes the progress information for the 2nd sample reflect the current progress
 * 
 * Sets the label and the slider maxes to the given msecs
 * 
 * \author Peter Grasch
 * \param int msecs
 * The progress
 */
void AddWordView::recSample2Status(int msecs)
{
	setRecStatus(ui.hsRec2, ui.lbRec2, msecs);
}


void AddWordView::setRecStatus(QSlider *prog, QLabel *textprog, int msecs)
{
	prog->setMaximum(msecs/100);
	QString tprog = makeTextProgress(msecs);
	textprog->setText("00:00 / "+tprog);
}
		
void AddWordView::setPlayStatus(QSlider *prog, QLabel *textprog, int msecs)
{
	prog->setValue(msecs/100);
	QString tprog = makeTextProgress(msecs);
	textprog->setText(  textprog->text().replace(0,textprog->text().indexOf(" /"),
			     tprog));
}

/**
 * \brief Makes an easily readable textprogress of the given time (in msecs)
 * 
 * e.g: 2.5 seconds (2500 msecs) would be translated to 2:50
 * \author Peter Grasch
 * \param int msecs
 * The msecs to tranlate
 * \return QString the converted QString
 */
QString AddWordView::makeTextProgress(int msecs)
{
	QString textprog = QString::number((int) msecs/10);
	
	textprog.insert(textprog.length()-2, ':');
	return textprog;
}

/**
 * \brief Makes the progress information for the 1st sample reflect the current progress
 * 
 * Sets the label and the slider values to the given msecs
 * 
 * \author Peter Grasch
 * \param int msecs
 * The progress
 */
void AddWordView::playSample1Status(int msecs)
{
	setPlayStatus(ui.hsRec1, ui.lbRec1, msecs);
}

/**
 * \brief Makes the progress information for the 2nd sample reflect the current progress
 * 
 * Sets the label and the slider values to the given msecs
 * 
 * \author Peter Grasch
 * \param int msecs
 * The progress
 */
void AddWordView::playSample2Status(int msecs)
{
	setPlayStatus(ui.hsRec2, ui.lbRec2, msecs);
}


/**
 * \brief Starts the recording with the given Elements
 * 
 * \author Peter Grasch
 * 
 * \param QString filename
 * This is the filename to record to (e.g.: 1.wav)
 */
void AddWordView::startRecording(QString filename)
{
	rec = new WavRecorder(this);
	rec->record(filename, 2, 44100); // hardcoded stereo, 44100hz
}

/**
 * \brief This will stop the current recording
 * 
 * Tells the wavrecorder to simply stop the recording and save the result.
 * \author Peter Grasch
 */
void AddWordView::stopRecording()
{
	if (!rec) return;
	rec->finish();
	disconnect(rec,0,0,0);
	delete rec;
	
	disconnect (ui.pbRec1, SIGNAL(clicked()), this, SLOT(stopRecording1()));
	disconnect (ui.pbRec2, SIGNAL(clicked()), this, SLOT(stopRecording2()));
	connect(ui.pbRec1, SIGNAL(clicked()), this, SLOT(recSample1()));
	connect(ui.pbRec2, SIGNAL(clicked()), this, SLOT(recSample2()));
}

/**
 * \brief This will stop the recording of sample 1
 * 
 * Calls the generic stopRecording function and disables the recording button
 * Enables the gui elements of the recording process of the second sample
 * 
 * \author Peter Grasch
 */
void AddWordView::stopRecording1()
{
	stopRecording();
	ui.pbPlay1->setEnabled(true);
	ui.pbRec1->setEnabled(false);
	ui.pbRec2->setEnabled(true);
	ui.hsRec2->setEnabled(true);
	ui.lbRec2->setEnabled(true);
	ui.pbStartOver1->setEnabled(true);
}

/**
 * \brief Deletes the first sample
 * 
 * Reinitializes the controls to a length of 00:00 and deletes the temp. file 1.wav
 * \author Peter Grasch
 */
void AddWordView::deleteSample1()
{
	ui.hsRec1->setMaximum(0);
	ui.lbRec1->setText("00:00 / 00:00");
	QFile f("1.wav");
	f.remove();
}

/**
 * \brief Deletes the second sample
 * 
 * Reinitializes the controls to a length of 00:00 and deletes the temp. file 2.wav
 * \author Peter Grasch
 */
void AddWordView::deleteSample2()
{
	ui.hsRec2->setMaximum(0);
	ui.lbRec2->setText("00:00 / 00:00");
	QFile f("2.wav");
	f.remove();
}


/**
 * \brief This will stop the recording of sample 2
 * 
 * Calls the generic stopRecording function and disables the recording button
 * \author Peter Grasch
 */
void AddWordView::stopRecording2()
{
	stopRecording();
	ui.pbRec2->setEnabled(false);
	ui.pbPlay2->setEnabled(true);
	ui.pbStartOver2->setEnabled(true);
}

void AddWordView::recSample1()
{
	this->startRecording("1.wav");
	
	disconnect(ui.pbRec1, SIGNAL(clicked()), this, SLOT(recSample1()));
	connect (ui.pbRec1, SIGNAL(clicked()), this, SLOT(stopRecording1()));
	disconnect(rec,0,0,0);
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(recSample1Status(int)));
}
void AddWordView::recSample2()
{
	this->startRecording("2.wav");
	disconnect(ui.pbRec2, SIGNAL(clicked()), this, SLOT(recSample2()));
	connect (ui.pbRec2, SIGNAL(clicked()), this, SLOT(stopRecording2()));
	disconnect(rec,0,0,0);
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(recSample2Status(int)));
}

/**
 * \brief Starts the playback of the given file
 * \author Peter Grasch
 * \param QString filename
 * The filename of the file to play
 */
void AddWordView::startPlayback(QString filename)
{
	play = new WavPlayer(this);
	play->play(filename);
}

/**
 * \brief Finishs the first playback
 * En-/Disables all the buttons, sets the slider and the progressbar and cleans up
 * \author Peter Grasch
 */
void AddWordView::finishPlayback1()
{
	ui.pbPlay1->setChecked(false);
	stopPlayback();
	playSample1Status(ui.hsRec1->maximum()*100);
}

/**
 * \brief Finishs the second playback
 * En-/Disables all the buttons, sets the slider and the progressbar and cleans up
 * \author Peter Grasch
 */
void AddWordView::finishPlayback2()
{
	ui.pbPlay2->setChecked(false);
	stopPlayback();
	playSample2Status(ui.hsRec2->maximum()*100);
}

/**
 * \brief Plays the first sample
 * \author Peter Grasch
 */
void AddWordView::playSample1()
{
	//testing
	startPlayback("1.wav");
	connect(play, SIGNAL(currentProgress(int)),this,SLOT(playSample1Status(int)));
	disconnect(ui.pbPlay1, SIGNAL(clicked()), this, SLOT(playSample1()));
	connect(ui.pbPlay1, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	
	connect(play, SIGNAL(finished()), this, SLOT(finishPlayback1()));
	connect(play, SIGNAL(terminated()), this, SLOT(finishPlayback1()));
	
}

/**
 * \brief Plays the second sample
 * \author Peter Grasch
 */
void AddWordView::playSample2()
{
	startPlayback("2.wav");
	connect(play, SIGNAL(currentProgress(int)),this,SLOT(playSample2Status(int)));
	disconnect(ui.pbPlay2, SIGNAL(clicked()), this, SLOT(playSample2()));
	connect(ui.pbPlay2, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	
	connect(play, SIGNAL(finished()), this, SLOT(finishPlayback2()));
	connect(play, SIGNAL(terminated()), this, SLOT(finishPlayback2()));
}

/**
 * \brief Stops the current playback by terminating the playing thread
 * \author Peter Grasch
 */
void AddWordView::stopPlayback()
{
	if (!play) return;
	play->stop();
	
	disconnect(play,0,0,0);
	
	disconnect(ui.pbPlay1, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	connect(ui.pbPlay1, SIGNAL(clicked()), this, SLOT(playSample1()));
	disconnect(ui.pbPlay2, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	connect(ui.pbPlay2, SIGNAL(clicked()), this, SLOT(playSample2()));
	
	delete play;
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

