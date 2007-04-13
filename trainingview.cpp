//
// C++ Implementation: trainingview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "trainingview.h"

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingView::TrainingView(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pbWordList, SIGNAL(clicked()), this, SLOT(switchToWordList()));
	connect(ui.pbTrainText, SIGNAL(clicked()), this, SLOT(trainSelected()));
	connect (ui.pbBackToMain, SIGNAL(clicked()), this, SLOT(cancelReading()));
	connect (ui.pbNextPage, SIGNAL(clicked()), this, SLOT(nextPage()));
	connect (ui.pbPrevPage, SIGNAL(clicked()), this, SLOT(prevPage()));
	
	connect(ui.pbRec, SIGNAL(clicked()), this, SLOT(recordPage()));
	connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(playSample()));
	connect(ui.pbStartOver, SIGNAL(clicked()), this, SLOT(deleteSample()));
	
	connect(ui.pbCancelTraining, SIGNAL(clicked()), this, SLOT(cancelTraining()));
	connect(ui.pbFinish, SIGNAL(clicked()), this, SLOT(finish()));
	
	currentPage=0;
	trainMgr = new TrainingManager();
	loadList();
}

/**
 * \brief Starts the recording with the given Elements
 * 
 * \author Peter Grasch
 * 
 * \param QString filename
 * This is the filename to record to (e.g.: 1.wav)
 */
void TrainingView::startRecording(QString filename)
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
void TrainingView::stopRecording()
{
	rec->finish();
	disconnect(rec,0,0,0);
	delete rec;
	ui.pbPrevPage->setEnabled(true);
	ui.pbNextPage->setEnabled(true);
	disconnect(ui.pbRec, SIGNAL(clicked()), this, SLOT(stopRecording()));
	connect(ui.pbRec, SIGNAL(clicked()), this, SLOT(recordPage()));
	
	//check if we are finished
	bool done=true;
	for (int i=0; i<trainMgr->getPageCount(); i++)
		if (lengthList->at(i) == 0) done=false;
	
	if (done) ui.pbFinish->setEnabled(true);
}

/**
 * \brief Sets the recording status with the given msecs
 * \author Peter Grasch
 * \param int msecs
 * The msecs to set the status to
 */
void TrainingView::setRecStatus(int msecs)
{
	ui.hsRec->setMaximum(msecs/100);
	QString tprog = makeTextProgress(msecs);
	ui.lbRec->setText("00:00 / "+tprog);
	this->lengthList->replace(currentPage,msecs);
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
QString TrainingView::makeTextProgress(int msecs)
{
	QString textprog = QString::number((int) msecs/10);
	
	textprog.insert(textprog.length()-2, ':');
	return textprog;
}

/**
 * \brief Records the current page
 * Uses the currentPage (member) variable to determine the current page and saves 
 * recording to the file rec<currentPage>.wav
 * \author Peter Grasch
 */
void TrainingView::recordPage()
{
	ui.pbPrevPage->setEnabled(false);
	ui.pbNextPage->setEnabled(false);
	startRecording(QString("rec")+QString::number(currentPage)+QString(".wav"));
	
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(setRecStatus(int)));
	
	disconnect(ui.pbRec, SIGNAL(clicked()), this, SLOT(recordPage()));
	connect(ui.pbRec, SIGNAL(clicked()), this, SLOT(stopRecording()));
}

/**
 * \brief Makes the Rec/Play/StartOver Buttons reflect the current Status
 * 
 * \author Peter Grasch
 */
void TrainingView::makeRecControlsReflectStatus()
{
	if (QFile(QString("rec")+QString::number(currentPage)+QString(".wav")).exists())
	{
		ui.pbRec->setEnabled(false);
		ui.pbPlay->setEnabled(true);
		ui.pbStartOver->setEnabled(true);
	} else {
		ui.pbRec->setEnabled(true);
		ui.pbPlay->setEnabled(false);
		ui.pbStartOver->setEnabled(false);
	}
	setRecStatus(lengthList->at(currentPage));
}


/**
 * \brief Starts the playback of the given file
 * \author Peter Grasch
 * \param QString filename
 * The filename of the file to play
 */
void TrainingView::startPlayback(QString filename)
{
	play = new WavPlayer(this);
	play->play(filename);
}

/**
 * \brief Make the progressbar reflect the current playback status
 * \author Peter Grasch
 */
void TrainingView::setPlayStatus(int msecs)
{
	ui.hsRec->setValue(msecs/100);
	QString tprog = makeTextProgress(msecs);
	ui.lbRec->setText(  ui.lbRec->text().replace(0,ui.lbRec->text().indexOf(" /"),
			    tprog));
}


/**
 * \brief Finishs the first playback
 * En-/Disables all the buttons, sets the slider and the progressbar and cleans up
 * \author Peter Grasch
 */
void TrainingView::stopPlayback()
{
	ui.pbPlay->setChecked(false);
	
	play->stop();
	
	disconnect(play,0,0,0);
	
	disconnect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(playSample()));
	delete play;
	
	ui.pbPrevPage->setEnabled(true);
	ui.pbNextPage->setEnabled(true);
	
	setPlayStatus(ui.hsRec->maximum()*100);
}



/**
 * \brief Records the current page
 * Uses the currentPage (member) variable to determine the current page and plays
 * the file rec<currentPage>.wav
 * \author Peter Grasch
 */
void TrainingView::playSample()
{
	ui.pbPrevPage->setEnabled(false);
	ui.pbNextPage->setEnabled(false);
	
	startPlayback(QString("rec")+QString::number(currentPage)+QString(".wav"));
	connect(play, SIGNAL(currentProgress(int)),this,SLOT(setPlayStatus(int)));
	disconnect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(playSample()));
	connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	
	connect(play, SIGNAL(finished()), this, SLOT(stopPlayback()));
	connect(play, SIGNAL(terminated()), this, SLOT(stopPlayback()));
}

/**
 * \brief Records the current page
 * Uses the currentPage (member) variable to determine the current page and deletes
 * the file rec<currentPage>.wav
 * \author Peter Grasch
 */
void TrainingView::deleteSample()
{
	ui.hsRec->setMaximum(0);
	ui.lbRec->setText("00:00 / 00:00");
	QFile f(QString("rec")+QString::number(currentPage)+QString(".wav"));
	f.remove();
	this->lengthList->replace(currentPage,0);
	//whenever we delete a sample, there is no way that all samples are recorded
	//any longer
	ui.pbFinish->setEnabled(false);
}

/**
 * \brief Starts the training of the selected text
 * \author Peter Grasch
 */
void TrainingView::trainSelected()
{
	if (ui.twTrainingWords->selectedItems().isEmpty())
	{
		QMessageBox::information(this,"Nichts ausgewählt","Bitte selektieren Sie zuerst einen Text aus der Liste.");
		return;
	}
	
	ui.swAction->setCurrentIndex(1);
	
	if (!(this-trainMgr->trainText(ui.twTrainingWords->currentRow()))) return;
	setWindowTitle(tr("Training - ")+trainMgr->getTextName());
	
	int count = trainMgr->getPageCount();
	ui.pbPages->setMaximum(count-1);
	lengthList = new QList<int>();
	for (int i=0; i < count; i++)
		lengthList->append(0);
	
	
	ui.pbFinish->setEnabled(false);
	this->currentPage=0;
	
	fetchPage(currentPage);
}

/**
 * \brief Trains the model with the gathered data
 * 
 * \author Peter Grasch
 */
void TrainingView::finish()
{
	ui.swAction->setCurrentIndex(2);
	ui.pbCompileModel->setValue(0);
	//trainMgr->setupTrainingSession();
}

/**
 * \brief Adapts the components to the content of the page given
 * Gets the content of the page and displays it in the label;
 * Displays the pageNumber in the progressbar and the groupbox-title
 * \author Peter Grasch
 * \param int page
 * The page to use
 * \note all information is retrieved from this->trainMgr
 */
void TrainingView::fetchPage(int page)
{
	ui.lbPage->setText( this->trainMgr->getPage(page) );
	ui.gbPage->setTitle("Seite: "+QString::number(page+1)+ " / "+ 
				QString::number(trainMgr->getPageCount()));
	ui.pbPages->setValue(page);
}

/**
 * \brief Jumps to the previous page in the pile
 * \author Peter Grasch
 */
void TrainingView::prevPage()
{
	if (currentPage>0)
		currentPage--;
	else return;
	fetchPage(currentPage);
	makeRecControlsReflectStatus();
}

/**
 * \brief Jumps to the next page in the pile
 * \author Peter Grasch
 */
void TrainingView::nextPage()
{
	int max = trainMgr->getPageCount()-1;
	if (currentPage < max)
		currentPage++;
	else return;
	fetchPage(currentPage);
	makeRecControlsReflectStatus();
}


/**
 * \brief Stops the training of a specific text and switches back to the overview
 * \author Peter Grasch
 * \todo cleaning up temp. stuff
 */
void TrainingView::cancelReading()
{
	ui.swAction->setCurrentIndex(0);
	setWindowTitle(tr("Training"));
}

/**
 * \brief Cancels the current Training
 * Tells the TrainingManager to abort building the new model (and to clean up)
 * It also goes back to the main list of trainingtexts
 * \author Peter Grasch
 */
void TrainingView::cancelTraining()
{
	this->trainMgr->pauseTraining();
	if (QMessageBox::question(this, "Wollen Sie wirklich abbrechen?", "Wenn Sie an diesem Punkt abbrechen, wird das Sprachmodell die in dieser Trainingseinheit gesammelten Daten verwerfen und die Erkennungsrate wird sich durch dieses Training nicht erhöhen.\n\nWollen Sie wirklich abbrechen?", QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
	{
		this->trainMgr->abortTraining();
		ui.swAction->setCurrentIndex(0);
		
		//cleaning up
		for (int i=0; i < trainMgr->getPageCount(); i++)
		{
// 			ui.hsRec->setMaximum(0);
// 			ui.lbRec->setText("00:00 / 00:00");
			QFile f(QString("rec")+QString::number(currentPage)+QString(".wav"));
			f.remove();
		}
	} else this->trainMgr->resumeTraining();
}


/**
 * @brief Switches to the WordList
 * 
 * Discards the Dialog and opens the Wordlist
 *
 *	@author Peter Grasch
 * @todo When we call reject() to close the dialog before we show the WordList we sometimes destroy the Dialog (and thus the WordListView handle) before we actually execute it - this causes a crash. This behaviour is now avoided by hiding the Dialog, showing the WordList and only after this execution returns we close the TrainingView
 */
void TrainingView::switchToWordList()
{
	WordListView *wlist = new WordListView(parentWidget());
	hide();
	wlist->exec();
	close();
}

/**
 * @brief Loads the List of known Trainingtexts
 * 
 * Asks the TrainingManager for the list and inserts it into the list
 *
 *	@author Peter Grasch
 */
void TrainingView::loadList()
{
	TrainingList *list = this->trainMgr->readTrainingTexts();

	if (!list) return;
	
	ui.twTrainingWords->setRowCount(list->count());
	
	for (int i=0; i<list->count(); i++)
	{
		ui.twTrainingWords->setItem(i, 0, new QTableWidgetItem(list->at(i)->getName()));
		ui.twTrainingWords->setItem(i, 1, new QTableWidgetItem(QString::number(list->at(i)->getPageCount())));
		ui.twTrainingWords->setItem(i, 2, new QTableWidgetItem(QString::number(list->at(i)->getRelevance())));
	}
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingView::~TrainingView()
{
}


