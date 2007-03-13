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
}

/**
 * @brief Switches to the WordList
 * 
 * Discards the Dialog and opens the Wordlist
 *
 *	@author Peter Grasch
 * @todo When we call reject() to close the dialog before we show the WordList we sometimes destroy the Dialog (and thus the WordListVie handle) before we actually execute it - this causes a crash. This behaviour is now avoided by hiding the Dialog, showing the WordList and only after this execution returns we close the TrainingView
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
	//TrainList *list = this->trainMgr->readTrainingTexts();
	//ui.twTrainingWords->setWordCount(trainingTexts->count());
}

// 	twVocab->setRowCount(vocab->count());
// 	
// 	for (int i=0; i<vocab->count(); i++)
// 	{
// 		twVocab->setItem(i, 0, new QTableWidgetItem(vocab->at(i)->getWord()));
// 		twVocab->setItem(i, 1, new QTableWidgetItem(*(vocab->at(i)->getPronunciation(0))));
// 		twVocab->setItem(i, 2, new QTableWidgetItem(vocab->at(i)->getTerminal()));
// 		
// 		QTableWidgetItem *prob = new QTableWidgetItem(QString().setNum(vocab->at(i)->getPropability()));
// 		if (vocab->at(i)->getPropability() == 0)
// 			prob->setBackgroundColor( QColor(255,0,0) );
// 		else 
// 			if (vocab->at(i)->getPropability() < 2)
// 				prob->setBackgroundColor( QColor( 241, 134, 134 ) );
// 		twVocab->setItem(i, 3, prob);
// 		
// 
// 		for (int j = 0; j<4; j++)
// 			twVocab->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
// 	}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingView::~TrainingView()
{
}


