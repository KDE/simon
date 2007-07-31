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
#include "importtrainingdirectory.h"

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingView::TrainingView(WordListView *wordlistView, QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pbWordList, SIGNAL(clicked()), this, SLOT(switchToWordList()));
	connect(ui.pbTrainText, SIGNAL(clicked()), this, SLOT(trainSelected()));
	
	connect (ui.pbNextPage, SIGNAL(clicked()), this, SLOT(nextPage()));
	connect (ui.pbPrevPage, SIGNAL(clicked()), this, SLOT(prevPage()));
	
	connect(ui.pbCancelTraining, SIGNAL(clicked()), this, SLOT(cancelTraining()));
	connect(ui.pbFinish, SIGNAL(clicked()), this, SLOT(finish()));
	connect(ui.pbImportText, SIGNAL(clicked()), this, SLOT(importTexts()));
		connect (ui.pbBackToMain, SIGNAL(clicked()), this, SLOT(cancelReading()));
		connect (ui.pbDelText, SIGNAL(clicked()), this, SLOT(deleteSelected()));
	connect(ui.pbImportDir, SIGNAL(clicked()), this, SLOT(importDirectory()));	
	
	currentPage=0;
	this->wordlistView = wordlistView;
	trainMgr = new TrainingManager(wordlistView->getManager());
	
	loadList(); // we load the list of avalible trainingtexts despite we probably won't
	// use it when given a special training program
	
// 	if (trainWords)
// 		this->trainWords(trainWords);
	
}


/**
 * \brief Deletes the selected text from the harddisc
 * 
 * Asks the user for confirmation befor the irreversible deletion
 * \author Peter Grasch
 */
void TrainingView::deleteSelected()
{
	if (ui.twTrainingWords->selectedItems().isEmpty())
	{
		QMessageBox::information(this,"Nichts ausgew�hlt","Bitte selektieren Sie zuerst einen Text aus der Liste.");
		return;
	}
	int currentIndex = ui.twTrainingWords->currentRow();
	if (!(this->trainMgr->trainText(currentIndex))) return;
	
	
	if (QMessageBox::question(this, "Wollen Sie den ausgew�hlten Text wirklich l�schen?", "Wenn Sie hier mit \"Ja\" best�tigen, wird der ausgew�hlte Text unwiederbringlich von der Festplatte gel�scht. Wollen Sie den ausgew�hlt wirklich l�schen?", QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
		this->trainMgr->deleteText(currentIndex);
	
	loadList();
}

/**
 * \brief Starts a special training with the given words
 * \author Peter Grasch
 */
void TrainingView::trainWords(WordList* words)
{
	if (!words) return;
	
	this->trainMgr->trainWords(words);
	
	startTraining();
}

/**
 * \brief Starts the training of the selected text
 * \author Peter Grasch
 */
void TrainingView::trainSelected()
{
	if (ui.twTrainingWords->selectedItems().isEmpty())
	{
		QMessageBox::information(this,"Nichts ausgew�hlt","Bitte selektieren Sie zuerst einen Text aus der Liste.");
		return;
	}
	
	if (!(this-trainMgr->trainText(ui.twTrainingWords->currentRow()))) return;
	
	startTraining();
}

/**
 * \brief Starts the training of the currently used text in the TrainingManager
 * 
 * Trains a text or a special Training Programm;
 * All the needed data is fetched from the concept class;
 * This ensures that the behaviour from the specialized training is not different from
 * the default training texts.
 * \author Peter Grasch
 */
void TrainingView::startTraining()
{
	ui.swAction->setCurrentIndex(1);
	setWindowTitle(tr("Training - ")+trainMgr->getTextName());
	
	int count = trainMgr->getPageCount();
	ui.pbPages->setMaximum(count-1);
	
	ui.pbFinish->setEnabled(false);
	this->currentPage=0;
	
	fetchPage(currentPage);
}

void TrainingView::importDirectory()
{
	ImportTrainingDirectory *importDir = new 
		ImportTrainingDirectory(this);
	hide();
	connect(importDir, SIGNAL(finished(int)), this, SLOT(show()));
	importDir->show();
	
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
	
	
	recorder = new RecWidget( "Seite: "+QString::number(page+1),
				  "sample"+QString::number(page+1)+".wav", ui.wRecTexts);
	ui.wRecTexts->layout()->addWidget(recorder);
	
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
	resetRecorder();
	fetchPage(currentPage);
}

void TrainingView::importTexts()
{
	ImportTrainingTexts *import = new ImportTrainingTexts();
	hide();
	import->start();
	connect(import, SIGNAL(finished(int)), this, SLOT(show()));
	connect(import, SIGNAL(finished(int)), this, SLOT(loadList()));
}

void TrainingView::resetRecorder()
{
	ui.wRecTexts->layout()->removeWidget(recorder);
	delete recorder;
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
	resetRecorder();
	fetchPage(currentPage);
}


/**
 * \brief Stops the training of a specific text and switches back to the overview
 * \author Peter Grasch
 * \todo cleaning up temp. stuff
 */
void TrainingView::cancelReading()
{
	ui.swAction->setCurrentIndex(0);
	delete recorder;
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
	if (QMessageBox::question(this, "Wollen Sie wirklich abbrechen?", "Wenn Sie an diesem Punkt abbrechen, wird das Sprachmodell die in dieser Trainingseinheit gesammelten Daten verwerfen und die Erkennungsrate wird sich durch dieses Training nicht erh�hen.\n\nWollen Sie wirklich abbrechen?", QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
	{
		this->trainMgr->abortTraining();
		ui.swAction->setCurrentIndex(0);
		
		//cleaning up
		for (int i=0; i < trainMgr->getPageCount(); i++)
		{
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
 */
void TrainingView::switchToWordList()
{
	reject();
	this->wordlistView->show();
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


