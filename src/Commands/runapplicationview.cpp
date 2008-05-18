//
// C++ Implementation: runapplicationview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "runapplicationview.h"
#include <QMessageBox>
#include <QIcon>
#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSize>
#include "../SimonLib/Settings/settings.h"
#include "../SimonLib/SimonInfo/simoninfo.h"
#include "runcommand.h"
#include "newcommand.h"

#ifdef __WIN32
#include "../SimonLib/WindowsLib/windowsresourcehandler.h"
#endif


/**
 *	@brief Constructor
 *
 *	@author Peter Grasch
 */
RunApplicationView::RunApplicationView(QWidget *parent) : InlineWidget(tr("Ausführen"), QIcon(":/images/icons/system-run.svg"), tr("Direkte ausführung von simon-Befehlen"), parent)
{
	ui.setupUi(this);
	guessChildTriggers((QObject*)this);

	setSettingsHidden();
	hide();

	connect ( ui.leSearch, SIGNAL(textChanged(QString)), this, SLOT(filterByPattern(QString)) );
	connect ( ui.pbClearSearch, SIGNAL(clicked()), ui.leSearch, SLOT(clear()) );
	connect ( ui.pbNewCommand, SIGNAL(clicked()), this, SLOT(addCommand()));
	connect ( ui.pbEditCommand, SIGNAL(clicked()), this, SLOT(editCommand()));
	connect ( ui.pbDeleteCommand, SIGNAL(clicked()), this, SLOT(deleteCommand()));

	registerControl(tr("Leere Suche"), ui.pbClearSearch, SLOT(animateClick()));

	this->loadCommands();
}


void RunApplicationView::setSettingsVisible()
{
	ui.pbNewCommand->show();
	ui.pbEditCommand->show();
	ui.pbDeleteCommand->show();
}

void RunApplicationView::setSettingsHidden()
{
	ui.pbNewCommand->hide();
	ui.pbEditCommand->hide();
	ui.pbDeleteCommand->hide();
}


void RunApplicationView::addCommand()
{
	NewCommand *newCommand = new NewCommand(this);
	if (newCommand->exec())
	{
		QMessageBox::information(this, tr("Füge Kommando hinzu"), tr("Jetz sollt i dann was machen..."));
	}
}

void RunApplicationView::editCommand()
{
	
}

void RunApplicationView::deleteCommand()
{
	if (QMessageBox::question(this, tr("Kommando löschen"), tr("Wollen Sie das ausgewählte Kommando wirklich unwiederruflich löschen?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
	{
		QMessageBox::information(this, tr("Kommando puttmachen"), tr("Jetz sollt i dann was machen..."));
	}
}


/**
 *	@brief Runs a command
 *	
 *	Basically extracts the name of the commando from the first
 *	tablewidget of the row of the argument and
 *	passes that command to the concept class
 *	
 *	We use the name of the command to determine what to do because
 *	this is exactly the same behavior we also use when we call it by voice
 *
 *	@param QTableWidgetItem* command
 *	Is used to determine the row
 *	
 *	@author Peter Grasch
 */
// void RunApplicationView::runCommand(QTableWidgetItem* command)
// {
// 	if (!ui.twPrograms->item(command->row(), 0)) return;
// 
// 	QString name = ui.twPrograms->item(command->row(), 0)->text();
// 	SimonInfo::showMessage(name, 2500, new QIcon(ui.twPrograms->item(command->row(), 0)->icon()));
// 	this->run->run(name);
// }


/**
 *	@brief Filters the List of commands (search) by the given string
 *	
 *	@author Peter Grasch
 *	@param QString filter
 *	The filter (case insensetive)
 */
void RunApplicationView::filterByPattern(QString filter)
{
// 	CommandList commands;
// 	if (!commlist)
// 		commands = run->getCommands();
// 	else commands = *commlist;
// 	
// 	for (int i = 0; i < commands.size(); i++)
// 	{
// 		if (commands.at(i)->getName().toUpper().indexOf( filter.toUpper(), Qt::CaseInsensitive ) == -1) {
// 			commands.removeAt(i);
// 			--i;
// 		}
// 	}
// 	commands =  filterByCategory(getCategory( ui.cbShow->currentText() ), &commands);
// 	insertCommands( commands );
}


/**
 *	@brief Gets the selected QTableWidgetItem and passes it to runCommand(...)
 *	
 *	@author Peter Grasch
 */
void RunApplicationView::runSelectedCommand()
{
// 	if ( ui.twPrograms->currentItem() )
// 		this->runCommand( ui.twPrograms->currentItem() );
}

/**
 *	@brief inserts the given Commands in the QTableWidget
 *
 *	@author Peter Grasch, Susanne Tschernegg
 */
void RunApplicationView::insertCommands ( CommandList list )
{
// 	int i=0;
// 	int type;
// 	QString strtype;
// 	ui.twPrograms->setRowCount ( list.count() );
// 	//ui.twPrograms->setItem(i,0, new QTableWidgetItem(QIcon(list.at(i)->getIconPath()),""));
// 
// 	for ( i=0; i<list.count(); i++ )
// 	{
// 		QString resourceId = list.at ( i )->getIconPath();
// 
// 		QIcon icon;
// #ifdef __WIN32
// 		if ( resourceId.contains ( QRegExp ( ".dll,\n*" ) ) )
// 		{
// 			QStringList iconResources = resourceId.split ( "," );
// 			WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
// 			icon = windowsResourceHandler->retrieveIcon ( iconResources.at ( 0 ), iconResources.at ( 1 ).toInt() );
// 		}
// 		else
// 		{
// 			QPixmap pixmap ( resourceId );
// 			icon.addPixmap ( pixmap );
// 		}
// #endif
// #ifndef __WIN32
// 		QPixmap pixmap ( resourceId );
// 		icon.addPixmap ( pixmap );
// #endif
// 
// 
// 		ui.twPrograms->setItem ( i, 0, new QTableWidgetItem ( icon, list.at ( i )->getName() ) );
// 
// 		type = list.at ( i )->getType();
// 		switch ( type )
// 		{
// 			case 0:
// 				strtype = "Ausführen";
// 				break;
// 			case 1:
// 				strtype = "Ort";
// 				break;
// 			case 2:
// 				strtype = "Sonderzeichen";
// 				break;
// 		}
// 
// 		ui.twPrograms->setItem ( i, 1, new QTableWidgetItem ( strtype ) );
// 		ui.twPrograms->setItem ( i, 2, new QTableWidgetItem ( list.at ( i )->getValue() ) );
// 		ui.twPrograms->setItem ( i, 3, new QTableWidgetItem ( list.at ( i )->getWorkingDirectory() ) );
// 		for ( int j = 0; j<4; j++ )
// 			ui.twPrograms->item ( i,j )->setFlags ( Qt::ItemIsSelectable|Qt::ItemIsEnabled );
// 	}
// 	ui.twPrograms->resizeColumnsToContents();
}

/**
*   \brief Loads and inserts the commandList by using the RunCommand backend
*
*   @author Peter Grasch
*/
void RunApplicationView::loadCommands()
{
    run = RunCommand::getInstance();
    run->reload();
    insertCommands (run->getCommands());
}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunApplicationView::~RunApplicationView()
{
	//do nothing - RunCommand should be preserved as it is a singleton...
}
