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
	
	trainMgr = new TrainingManager();
	loadList();
}

/**
 * \brief Starts the training of the selected text
 * \author Peter Grasch
 */
void TrainingView::trainSelected()
{
	ui.swAction->setCurrentIndex(1);
	TrainingText *text = this->trainMgr->getText(ui.twTrainingWords->currentRow());
	if (!text) return;
	ui.lbPage->setText( text->getPage(0) );
}

/**
 * \brief Stops the training of a specific text and switches back to the overview
 * \author Peter Grasch
 * \todo cleaning up temp. stuff
 */
void TrainingView::cancelReading()
{
	ui.swAction->setCurrentIndex(0);
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


