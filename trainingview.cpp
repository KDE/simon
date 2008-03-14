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
#include <QDir>
#include "trainingview.h"
#include "importtrainingdirectory.h"
#include "settings.h"
#include <QHash>
#include "settings.h"
#include <QHashIterator>
#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include "simoninfo.h"
#include "trainingtext.h"
#include "trainingmanager.h"
#include "trainingtext.h"
#include "importtrainingtexts.h"
#include "recwidget.h"
#include "addwordview.h"


TrainingView* TrainingView::instance;

/**
 * \brief Constructor - inits the Gui
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
TrainingView::TrainingView ( QWidget *parent )
		: InlineWidget ( tr ( "Training" ), QIcon ( ":/images/icons/document-properties.svg" ),
		                 tr ( "Trainieren des Sprachmodells" ), parent )
{
	ui.setupUi ( this );
	recorder=0;
	guessChildTriggers ( ( QObject* ) this );
	this->hide();

	connect ( ui.pbTrainText, SIGNAL ( clicked() ), this, SLOT ( trainSelected() ) );
	connect ( ui.twTrainingWords, SIGNAL ( cellDoubleClicked ( int,int ) ), this, SLOT ( trainSelected() ) );

	ui.pbPrevPage->setEnabled ( false );
	connect ( ui.pbNextPage, SIGNAL ( clicked() ), this, SLOT ( nextPage() ) );
	connect ( ui.pbPrevPage, SIGNAL ( clicked() ), this, SLOT ( prevPage() ) );

	connect ( ui.pbFinish, SIGNAL ( clicked() ), this, SLOT ( finish() ) );
	connect ( ui.pbImportText, SIGNAL ( clicked() ), this, SLOT ( importTexts() ) );
	connect ( ui.pbBackToMain, SIGNAL ( clicked() ), this, SLOT ( cancelReading() ) );
	connect ( ui.pbDelText, SIGNAL ( clicked() ), this, SLOT ( deleteSelected() ) );
	connect ( ui.pbImportDir, SIGNAL ( clicked() ), this, SLOT ( importDirectory() ) );

	currentPage=0;
	import = new ImportTrainingTexts();;
	trainMgr = TrainingManager::getInstance();
	loadList();
}


/**
 * \brief Deletes the selected text from the harddisc
 *
 * Asks the user for confirmation befor the irreversible deletion
 * \author Peter Grasch
 */
void TrainingView::deleteSelected()
{
	if ( ui.twTrainingWords->selectedItems().isEmpty() )
	{
		QMessageBox::information ( this,tr ( "Nichts ausgewählt" ),tr ( "Bitte selektieren Sie zuerst einen Text aus der Liste." ) );
		return;
	}
	int currentIndex = ui.twTrainingWords->currentRow();

	if ( QMessageBox::question ( this, tr ( "Wollen Sie den ausgewählten Text wirklich löschen?" ), tr ( "Wenn Sie hier mit \"Ja\" bestätigen, wird der ausgewählte Text unwiderbringlich von der Festplatte gelöscht. Wollen Sie den ausgewählten Text wirklich löschen?" ), QMessageBox::Yes|QMessageBox::No ) ==QMessageBox::Yes )
		this->trainMgr->deleteText ( currentIndex );

	loadList();
}

/**
 * \brief Starts a special training with the given words
 * \author Peter Grasch
 */
void TrainingView::trainWords ( WordList* words )
{
	if ( !words ) return;

	this->trainMgr->trainWords ( words );

	startTraining();
}

/**
 * \brief Starts the training of the selected text
 * \author Peter Grasch
 */
void TrainingView::trainSelected()
{
	recordedPages = 0;
	if ( ui.twTrainingWords->selectedItems().isEmpty() )
	{
		QMessageBox::information ( this,tr ( "Nichts ausgewählt" ),tr ( "Bitte selektieren Sie zuerst einen Text aus der Liste." ) );
		return;
	}
	bool success = trainMgr->trainText ( ui.twTrainingWords->currentRow() );
	if ( !success )
	{
// 		QMessageBox::critical(this, tr("Konnte Training nicht starten"), tr("Konnte Training nicht starten.\n\nDer Text konnte nicht geladen werden."));
		SimonInfo::showMessage(tr("Konnte Training nicht starten"), 2000); // show passive notification
		return;
	}

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
	ui.swAction->setCurrentIndex ( 1 );
	setWindowTitle ( tr ( "Training - " ) +trainMgr->getTextName() );

	int count = trainMgr->getPageCount();
	ui.pbPages->setMaximum ( count-1 );

	ui.pbFinish->setEnabled ( false );
	this->currentPage=0;

	fetchPage ( currentPage );
}

/**
 * \brief Shows the ImportTrainingDirectory-Wizard
 * \author Peter Grasch
 */
void TrainingView::importDirectory()
{
	ImportTrainingDirectory *importDir = new ImportTrainingDirectory ( this );
	importDir->show();

}

/**
 * \brief Trains the model with the gathered data
 *
 * \author Peter Grasch
 */
void TrainingView::finish()
{
	ui.swAction->setCurrentIndex ( 2 );
// 	ui.pbCompileModel->setValue(0);

	//training...


	//finishing up

	trainMgr->finishTrainingSession();

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
void TrainingView::fetchPage ( int page )
{
	ui.lbPage->setText ( this->trainMgr->getPage ( page ) );
	QHashIterator<QString, QString> hIterator ( *trainMgr->getSampleHash() );
	hIterator.toFront();
	QString keyStr;
	int i = 0;
	while ( hIterator.hasNext() )
	{
		hIterator.next();
		if ( i==page )
		{
			keyStr = hIterator.value();
			break;
		}
		i++;
	}

	QString value = Settings::getS ( "Model/PathToSamples" ) +"/"+keyStr;
	resetRecorder();
	recorder = new RecWidget ( tr ( "Seite: %1" ).arg ( page+1 ),
	                           value, ui.wRecTexts );  //<name-des-textes>_S<seitennummer>_<datum/zeit>.wav

	connect ( recorder, SIGNAL ( recordingFinished() ), this, SLOT ( increaseRecordedPages() ) );
	connect ( recorder, SIGNAL ( sampleDeleted() ), this, SLOT ( decreaseRecordedPages() ) );

	ui.wRecTexts->layout()->addWidget ( recorder );

	ui.gbPage->setTitle ( tr ( "Seite: %1/%2" ).arg ( page+1 ).arg ( trainMgr->getPageCount() ) );
	ui.pbPages->setValue ( page );
}

/**
 * \brief Jumps to the previous page in the pile
 * \author Peter Grasch, Susanne Tschernegg
 */
void TrainingView::prevPage()
{
	if ( currentPage>0 )
	{
		if ( currentPage==1 )
			ui.pbPrevPage->setEnabled ( false );
		else if ( ui.pbPrevPage->isEnabled() ==false )
			ui.pbPrevPage->setEnabled ( true );
		currentPage--;
	}
	else
	{
		ui.pbPrevPage->setEnabled ( false );
	}
	int max = trainMgr->getPageCount()-1;
	if ( currentPage < max )
	{
		ui.pbNextPage->setEnabled ( true );
	}
	else
	{
		ui.pbNextPage->setEnabled ( false );
	}
	resetRecorder();
	fetchPage ( currentPage );
}

/**
 * \brief Displays the ImportTrainingTexts Wizard
 * \author Peter Grasch
 */
void TrainingView::importTexts()
{
	if (import->isVisible()) return;
	import->restart();
	import->start();
	connect ( import, SIGNAL ( finished ( int ) ), this, SLOT ( loadList() ) );
}

/**
 * \brief Resets the RecordingWidget
 * \author Peter Grasch
 */
void TrainingView::resetRecorder()
{
	if (!recorder) return;

	ui.wRecTexts->layout()->removeWidget ( recorder );
	delete recorder;
	recorder=0;
}

/**
 * \brief Jumps to the next page in the pile
 * \author Peter Grasch, Susanne Tschernegg
 */
void TrainingView::nextPage()
{
	int max = trainMgr->getPageCount()-1;
	if ( currentPage < max )
	{
		if ( currentPage==max-1 )
			ui.pbNextPage->setEnabled ( false );
		else if ( ui.pbNextPage->isEnabled() ==false )
			ui.pbNextPage->setEnabled ( true );
		currentPage++;
	}
	else
	{
		ui.pbNextPage->setEnabled ( false );
	}
	if ( currentPage > 0 )
	{
		ui.pbPrevPage->setEnabled ( true );
	}
	resetRecorder();
	fetchPage ( currentPage );
}


/**
 * \brief Stops the training of a specific text and switches back to the overview
 * \author Peter Grasch, Susanne Tschernegg
 * \todo cleaning up temp. stuff
 */
void TrainingView::cancelReading()
{
	cleanUpTrainingSamples();
	ui.swAction->setCurrentIndex ( 0 );
	resetRecorder();
	setWindowTitle ( tr ( "Training" ) );
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
 * \author Peter Grasch, Susanne Tschernegg
 */
void TrainingView::cancelTraining()
{
	if ( QMessageBox::question ( this, tr ( "Wollen Sie wirklich abbrechen?" ), tr ( "Wenn Sie an diesem Punkt abbrechen, wird das Sprachmodell die in dieser Trainingseinheit gesammelten Daten verwerfen und die Erkennungsrate wird sich durch dieses Training nicht erhöhen.\n\nWollen Sie wirklich abbrechen?" ), QMessageBox::Yes|QMessageBox::No ) ==QMessageBox::Yes )
	{
		cleanUpTrainingSamples();
	}
}

/**
 * @brief cleans up the saved Trainingfiles, because they are no longer needed
 *
 *	@author Peter Grasch, Susanne Tschernegg
*/
void TrainingView::cleanUpTrainingSamples()
{
	ui.swAction->setCurrentIndex ( 0 );

	//cleaning up
	for ( int i=1; i < trainMgr->getPageCount() +1; i++ )
	{
		QDir dir ( Settings::getS ( "Model/PathToSamples" ) );
		//QStringList list = dir.entryList(QDir::Files);
		/*QString textName = trainMgr->getTextName();
		textName.replace(QString(" "), QString("_"));
		QStringList filteredList = list.filter(QRegExp(QString(textName+"_S"+QString::number(i)+"_"+QString(qvariant_cast<QString>(QDate::currentDate()))+"_*.wav")));*/
		QHashIterator<QString, QString> hIterator ( *trainMgr->getSampleHash() );
		while ( hIterator.hasNext() )
		{
			hIterator.next();
			QFile f ( hIterator.value() );
			if ( f.exists() )
				f.remove();
		}
		trainMgr->getSampleHash()->clear();
	}
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

	if ( !list ) return;

	ui.twTrainingWords->setRowCount ( list->count() );

	for ( int i=0; i<list->count(); i++ )
	{
		ui.twTrainingWords->setItem ( i, 0, new QTableWidgetItem ( list->at ( i )->getName() ) );
		ui.twTrainingWords->setItem ( i, 1, new QTableWidgetItem ( QString::number ( list->at ( i )->getPageCount() ) ) );
		ui.twTrainingWords->setItem ( i, 2, new QTableWidgetItem ( QString::number ( list->at ( i )->getRelevance() ) ) );

		//make them readonly
		for ( int j = 0; j<3; j++ )
			ui.twTrainingWords->item ( i,j )->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEnabled );
	}
	ui.twTrainingWords->resizeColumnToContents ( 0 );
	//list does not need to be deleted
	//it is a member variable of the underlying concept class and will be scheduled for deletion ones we destroy it
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingView::~TrainingView()
{
    recorder->deleteLater();
    import->deleteLater();
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
 * @brief increases the recorded pages. if the recordedpages the same as the maximum of pages, the finish-butten will be enabled.
 *
 *	@author Susanne Tschernegg
 */
void TrainingView::increaseRecordedPages()
{
	int max = trainMgr->getPageCount();
	recordedPages++;
	if ( recordedPages==max )
		ui.pbFinish->setEnabled ( true );
}

/**
 * @brief decreases the recorded pages. if the recordedpages were before the same as the maximum of pages, the finish-butten will be enabled.
 *
 *	@author Susanne Tschernegg
 */
void TrainingView::decreaseRecordedPages()
{
	int max = trainMgr->getPageCount();
	if ( recordedPages==max )
		ui.pbFinish->setEnabled ( false );
}
