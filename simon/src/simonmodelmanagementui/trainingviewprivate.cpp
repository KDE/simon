/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "trainingviewprivate.h"


#include "ImportTrainingData/importtrainingdirectory.h"
#ifdef SIMON_SCENARIOS
#include <speechmodelmanagement_scenario/trainingmanager.h>
#else
#include <speechmodelmanagement/trainingmanager.h>
#endif
#include "ImportTrainingTexts/importtrainingtexts.h"
#include "TrainSamples/trainingswizard.h"

#include <simonsound/recwidget.h>
#include <simoninfo/simoninfo.h>

#include <QDir>
#include <QHash>
#include <QHashIterator>
#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include <KMessageBox>


/**
 * \brief Constructor - inits the Gui
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
TrainingViewPrivate::TrainingViewPrivate ( QWidget *parent )
	: QWidget(parent),
	import(new ImportTrainingTexts())
{
	TrainingManager::getInstance()->init();

	ui.setupUi ( this );

	connect ( ui.pbTrainText, SIGNAL ( clicked() ), this, SLOT ( trainSelected() ) );
	connect ( ui.twTrainingWords, SIGNAL ( cellDoubleClicked ( int,int ) ), this, SLOT ( trainSelected() ) );

	connect ( ui.pbImportText, SIGNAL ( clicked() ), this, SLOT ( importTexts() ) );
	connect ( ui.pbDelText, SIGNAL ( clicked() ), this, SLOT ( deleteSelected() ) );
	connect ( ui.pbImportDir, SIGNAL ( clicked() ), this, SLOT ( importDirectory() ) );

	connect(TrainingManager::getInstance(), SIGNAL(trainingDataChanged()), this, SLOT(loadList()));
	loadList();


	//set up icons
	ui.pbTrainText->setIcon(KIcon("go-next"));
	ui.pbDelText->setIcon(KIcon("edit-delete"));
	ui.pbImportText->setIcon(KIcon("document-import"));
	ui.pbImportDir->setIcon(KIcon("document-open-folder"));
}


/**
 * \brief Deletes the selected text from the harddisc
 *
 * Asks the user for confirmation before the irreversible deletion
 * \author Peter Grasch
 */
void TrainingViewPrivate::deleteSelected()
{
	if ( ui.twTrainingWords->selectedItems().isEmpty() )
	{
		KMessageBox::information ( this ,i18n ( "Please select a text from the list." ) );
		return;
	}
	int currentIndex = ui.twTrainingWords->currentRow();

	if ( KMessageBox::questionYesNoCancel ( this, i18n ( "If you confirm this dialog, the selected Text will be completely removed "
"from your hard drive.\n\nDo you want to continue?")) == KMessageBox::Yes )
		if (!TrainingManager::getInstance()->deleteText ( currentIndex ))
			KMessageBox::sorry(this, i18n("Couldn't delete Trainingstext. Do you have enough permissions?"));

	loadList();
}


/**
 * \brief Starts the training of the selected text
 * \author Peter Grasch
 */
void TrainingViewPrivate::trainSelected()
{
	if ( ui.twTrainingWords->selectedItems().isEmpty() )
	{
		KMessageBox::information(this,i18n("Please select a text from the list."));
		return;
	}
	TrainingText *text =TrainingManager::getInstance()->getText ( ui.twTrainingWords->currentRow() );
	if ( !text )
	{
		SimonInfo::showMessage(i18n("Couldn't start Training"), 2000); // show passive notification
		return;
	}

	TrainingsWizard *wizard = new TrainingsWizard(this);
	if (wizard->init(*text))
		wizard->exec();
	wizard->deleteLater();
}


/**
 * \brief Shows the ImportTrainingDirectory-Wizard
 * \author Peter Grasch
 */
void TrainingViewPrivate::importDirectory()
{
	ImportTrainingDirectory *importDir = new ImportTrainingDirectory ( this );
	importDir->show();

}


/**
 * \brief Displays the ImportTrainingTexts Wizard
 * \author Peter Grasch
 */
void TrainingViewPrivate::importTexts()
{
	if (import->isVisible()) return;
	import->restart();
	import->start();
	connect ( import, SIGNAL ( finished ( int ) ), this, SLOT ( loadList() ) );
}


/**
 * @brief Loads the List of known Trainingtexts
 *
 * Asks the TrainingManager for the list and inserts it into the list
 *
 *	@author Peter Grasch
 */
void TrainingViewPrivate::loadList()
{
	TrainingList *list = TrainingManager::getInstance()->readTrainingTexts();

	if ( !list ) return;

	ui.twTrainingWords->setRowCount ( list->count() );

	for ( int i=0; i<list->count(); i++ )
	{
		ui.twTrainingWords->setItem ( i, 0, new QTableWidgetItem ( list->at ( i )->getName() ) );
		int pageCount = list->at ( i )->getPageCount();
		ui.twTrainingWords->setItem ( i, 1, new QTableWidgetItem ( QString::number ( pageCount ) ));
		ui.twTrainingWords->setItem ( i, 2, new QTableWidgetItem ( QString::number ( list->at ( i )->getRelevance() ) ) );

		QString tooltip;
		
		if (pageCount > 0)
			tooltip = i18n("Preview: %1...", list->at(i)->getPage(0));
		else tooltip = i18n("Empty");
		
		//make them readonly
		for ( int j = 0; j<3; j++ )
		{
			ui.twTrainingWords->item ( i,j )->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEnabled );
			ui.twTrainingWords->item ( i,j )->setToolTip(tooltip);
		}
	}
	ui.twTrainingWords->resizeColumnsToContents ( );
	//list does not need to be deleted
	//it is a member variable of the underlying concept class and will be scheduled for deletion ones we destroy it
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingViewPrivate::~TrainingViewPrivate()
{
    import->deleteLater();
}

