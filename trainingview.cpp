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
#include <QHBoxLayout>
#include "trainingview.h"
#include "importtrainingdirectory.h"
#include "settings.h"


/**
 * \brief Constructor - inits the Gui
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
TrainingView::TrainingView(QWidget *parent)
	 : InlineWidget(tr("Training"), QIcon(":/images/icons/document-properties.svg"), 
	   tr("Trainieren des Sprachmodells"), parent)
{
	ui.setupUi(this);
	this->hide();
	
	connect(ui.pbTrainText, SIGNAL(clicked()), this, SLOT(trainSelected()));
	connect(ui.twTrainingWords, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(trainSelected()));
	
	ui.pbPrevPage->setEnabled(false);
	connect (ui.pbNextPage, SIGNAL(clicked()), this, SLOT(nextPage()));
	connect (ui.pbPrevPage, SIGNAL(clicked()), this, SLOT(prevPage()));
	
	connect(ui.pbCancelTraining, SIGNAL(clicked()), this, SLOT(cancelTraining()));
	connect(ui.pbFinish, SIGNAL(clicked()), this, SLOT(finish()));
	connect(ui.pbImportText, SIGNAL(clicked()), this, SLOT(importTexts()));
	connect (ui.pbBackToMain, SIGNAL(clicked()), this, SLOT(cancelReading()));
	connect (ui.pbDelText, SIGNAL(clicked()), this, SLOT(deleteSelected()));
	connect(ui.pbImportDir, SIGNAL(clicked()), this, SLOT(importDirectory()));	
	
	currentPage=0;
	trainMgr = new TrainingManager();
	
	loadList(); // we load the list of avalible trainingtexts despite we probably won't
	// use it when given a special training program
    
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
		QMessageBox::information(this,tr("Nichts ausgewählt"),tr("Bitte selektieren Sie zuerst einen Text aus der Liste."));
		return;
	}
	int currentIndex = ui.twTrainingWords->currentRow();
	if (!(this->trainMgr->trainText(currentIndex))) return;
	
	
	if (QMessageBox::question(this, tr("Wollen Sie den ausgewählten Text wirklich löschen?"), tr("Wenn Sie hier mit \"Ja\" bestätigen, wird der ausgewählte Text unwiderbringlich von der Festplatte gelöscht. Wollen Sie den ausgewählten Text wirklich löschen?"), QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
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
    recordedPages = 0;
	if (ui.twTrainingWords->selectedItems().isEmpty())
	{
		QMessageBox::information(this,tr("Nichts ausgewählt"),tr("Bitte selektieren Sie zuerst einen Text aus der Liste."));
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

/**
 * \brief Shows the ImportTrainingDirectory-Wizard
 * \author Peter Grasch
 */
void TrainingView::importDirectory()
{
	ImportTrainingDirectory *importDir = new 
		ImportTrainingDirectory(this);
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

	//training...


	//finishing up

	//trainMgr->setupTrainingSession();

	//done
	emit trainingCompleted();
}

/**
 * \brief Adapts the components to the content of the page given
 * Gets the content of the page and displays it in the label;
 * Displays the pageNumber in the progressbar and the groupbox-title
 * \author Peter Grasch, Susanne Tschernegg
 * \param int page
 * The page to use
 * \note all information is retrieved from this->trainMgr
 */
void TrainingView::fetchPage(int page)
{
	ui.lbPage->setText( this->trainMgr->getPage(page) );
	
	
	recorder = new RecWidget( tr("Seite: %1").arg(page+1),
				  tr("sample%1.wav").arg(page+1), ui.wRecTexts);

    connect(recorder, SIGNAL(recordingFinished()), this, SLOT(increaseRecordedPages()));
    connect(recorder, SIGNAL(sampleDeleted()), this, SLOT(decreaseRecordedPages()));
    
	ui.wRecTexts->layout()->addWidget(recorder);
	
	ui.gbPage->setTitle(tr("Seite: %1/%2").arg(page+1).arg(trainMgr->getPageCount()));
	ui.pbPages->setValue(page);
}

/**
 * \brief Jumps to the previous page in the pile
 * \author Peter Grasch, Susanne Tschernegg
 */
void TrainingView::prevPage()
{
	if (currentPage>0)
    {
        if(currentPage==1)
            ui.pbPrevPage->setEnabled(false);
        else if(ui.pbPrevPage->isEnabled()==false)
            ui.pbPrevPage->setEnabled(true);
		currentPage--;
    }
	else
    {   
        ui.pbPrevPage->setEnabled(false);
    }
    int max = trainMgr->getPageCount()-1;
    if(currentPage < max)
    {
        ui.pbNextPage->setEnabled(true);
    }
    else
    {
        ui.pbNextPage->setEnabled(false);
    }
	resetRecorder();
	fetchPage(currentPage);
}

/**
 * \brief Displays the ImportTrainingTexts Wizard
 * \author Peter Grasch
 */
void TrainingView::importTexts()
{
	ImportTrainingTexts *import = new ImportTrainingTexts();
	import->start();
	connect(import, SIGNAL(finished(int)), this, SLOT(loadList()));
}

/**
 * \brief Resets the RecordingWidget
 * \author Peter Grasch
 */
void TrainingView::resetRecorder()
{
	ui.wRecTexts->layout()->removeWidget(recorder);
	delete recorder;
}

/**
 * \brief Jumps to the next page in the pile
 * \author Peter Grasch, Susanne Tschernegg
 */
void TrainingView::nextPage()
{
	int max = trainMgr->getPageCount()-1;
	if (currentPage < max)
    {
        if(currentPage==max-1)
            ui.pbNextPage->setEnabled(false);
        else if(ui.pbNextPage->isEnabled()==false)
            ui.pbNextPage->setEnabled(true);
		currentPage++;
    }
	else
    {
        ui.pbNextPage->setEnabled(false);
    }
    if(currentPage > 0)
    {
        ui.pbPrevPage->setEnabled(true);
    }
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
 * \brief Emits the displayMe Signal
 * \author Peter Grasch
 */
void TrainingView::exec()
{
	emit displayMe();
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
	if (QMessageBox::question(this, tr("Wollen Sie wirklich abbrechen?"), tr("Wenn Sie an diesem Punkt abbrechen, wird das Sprachmodell die in dieser Trainingseinheit gesammelten Daten verwerfen und die Erkennungsrate wird sich durch dieses Training nicht erhöhen.\n\nWollen Sie wirklich abbrechen?"), QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
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
		
		//make them readonly
		for (int j = 0; j<3; j++)
			ui.twTrainingWords->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
	ui.twTrainingWords->resizeColumnToContents(0);
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingView::~TrainingView()
{
}

/**
 * @brief to hide the settings
 *
 *	@author Susanne Tschernegg
 */
void TrainingView::hideSettings()
{
    ui.pbDelText->hide();
    ui.pbImportText->hide();
    ui.pbImportDir->hide();
}

/**
 * @brief to set the settings visible
 *
 *	@author Susanne Tschernegg
 */
void TrainingView::setSettingsVisible()
{
    ui.pbDelText->show();
    ui.pbImportText->show();
    ui.pbImportDir->show();
}

/**
 * @brief
 *
 *	@author Susanne Tschernegg
 */
void TrainingView::increaseRecordedPages()
{
    QMessageBox::information(this,"askdgkdfdsk ","trainingsview incrase");
    int max = trainMgr->getPageCount();
    QMessageBox::information(this,"ajksddasf", qvariant_cast<QString>(max));
    recordedPages++;
    QMessageBox::information(this,"ajksddasf", qvariant_cast<QString>(recordedPages));
    if(recordedPages==max)
        ui.pbFinish->setEnabled(true);
}

/**
 * @brief
 *
 *	@author Susanne Tschernegg
 */
void TrainingView::decreaseRecordedPages()
{
    int max = trainMgr->getPageCount();
    if(recordedPages==max)
        ui.pbFinish->setEnabled(false);
    recordedPages--;
}

