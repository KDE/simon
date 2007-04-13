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
	
	currentPage=0;
	trainMgr = new TrainingManager();
	loadList();
}

/**
 * \brief Records the current page
 * Uses the currentPage (member) variable to determine the current page and saves 
 * recording to the file rec<currentPage>.wav
 * \author Peter Grasch
 */
void TrainingView::recordPage()
{
	
}

/**
 * \brief Makes the Rec/Play/StartOver Buttons reflect the current Status
 * 
 * \author Peter Grasch
 */
void TrainingView::makeRecControlsReflectStatus()
{
	if (QFile(QString("rec")+currentPage+QString(".wav")).exists())
	{
		ui.pbRec->setEnabled(false);
		ui.pbPlay->setEnabled(true);
		ui.pbStartOver->setEnabled(true);
	} else {
		ui.pbRec->setEnabled(true);
		ui.pbPlay->setEnabled(false);
		ui.pbStartOver->setEnabled(false);
	}
}

/**
 * \brief Records the current page
 * Uses the currentPage (member) variable to determine the current page and plays
 * the file rec<currentPage>.wav
 * \author Peter Grasch
 */
void TrainingView::playSample()
{
	
}

/**
 * \brief Records the current page
 * Uses the currentPage (member) variable to determine the current page and deletes
 * the file rec<currentPage>.wav
 * \author Peter Grasch
 */
void TrainingView::deleteSample()
{
	
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
	ui.pbPages->setMaximum(trainMgr->getPageCount());
	
	currentPage=0;
	
	fetchPage(currentPage);
	
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


