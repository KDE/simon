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
#include "settings.h"
#include <QMessageBox>
#include <QIcon>
#include "windowsresourcehandler.h"
#include "iconbutton.h"

/**
 *	@brief Constructor
 *
 *	@author Peter Grasch
 */
RunApplicationView::RunApplicationView(RunCommand *run, QWidget *parent) : InlineWidget(tr("Ausführen"), QIcon(":/images/icons/emblem-system"), tr("Direkte ausführung von simon-Befehlen"), parent)
{
	ui.setupUi(this);
	hide();
    this->run = run;
	ui.twPrograms->verticalHeader()->hide();
	insertCommands ( run->getCommands() );
	connect(ui.twPrograms, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(runCommand(QTableWidgetItem*)));
	connect(ui.pbRun, SIGNAL(clicked()), this, SLOT(runSelectedCommand()));
	connect ( ui.leSearch, SIGNAL(textEdited(QString)), this, SLOT(filterByPattern(QString)) );
	connect ( ui.cbShow, SIGNAL(currentIndexChanged(QString)), this, SLOT(filterByPattern()) );
	connect ( ui.pbClearSearch, SIGNAL(clicked()), this, SLOT(clearSearchText()) );
    
    //connect ( , SIGNAL(commandsChanged()), this, SLOT(loadCommands()));
}


/**
 *	@brief Clears the search-lineedit
 *
 *	@author Peter Grasch
 */
void RunApplicationView::clearSearchText()
{
	ui.leSearch->setText("");
	filterByPattern();
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
void RunApplicationView::runCommand(QTableWidgetItem* command)
{
    
	QString name = ui.twPrograms->item(command->row(), 1)->text();
	SimonInfo::showMessage(name, 2500);
	this->run->run(name);
}


/**
 *	@brief Filters the List of commands (search) by the given string
 *	
 *	If uses the CommandList and deletes all commands that doesn't conform to the pattern
 *	
 *	@author Peter Grasch
 *	@param QString filter
 *	The filter - this is case insensitive
 *	@param CommandList *commlist
 *	If supplied it contains the list to be filtered, if not the list is fetched from the Concept layer
 */
void RunApplicationView::filterByPattern(QString filter, CommandList *commlist)
{
	CommandList commands;
	if (!commlist)
		commands = run->getCommands();
	else commands = *commlist;
	
	for (int i = 0; i < commands.size(); i++)
	{
		if (commands.at(i)->getName().toUpper().indexOf( filter.toUpper(), Qt::CaseInsensitive ) == -1) {
			commands.removeAt(i);
			--i;
		}
	}
	commands =  filterByCategory(getCategory( ui.cbShow->currentText() ), &commands);
	insertCommands( commands );
}

/**
 *	@brief Overloaded function: Filters the List of commands (search) by the given string
 *	
 *	Calls the filterByPattern function and supplies the text from the Search-Line Edit
 *	
 *	@author Peter Grasch
 */
void RunApplicationView::filterByPattern()
{
	filterByPattern(ui.leSearch->text());
}


/**
 *	@brief Filters the List of commands (search) by the given category
 *	
 *	If uses the CommandList and deletes all commands that doesn't conform to the category
 *	
 *	@author Peter Grasch
 *	@param int commandtype
 *	commandtype (0=exec, 1=place, 2=type)
 *	@param CommandList *commlist
 *	If supplied it contains the list to be filtered, if not the list is fetched from the Concept layer
 */
CommandList RunApplicationView::filterByCategory(int commandtype, CommandList *commlist)
{
	CommandList commands;
	if (!commlist)
		commands = run->getCommands();
	else commands = *commlist;
	
	for (int i = 0; i < commands.size(); i++)
	{
		if ((commandtype != 3) && (commands.at(i)->getType() != commandtype)) {
			commands.removeAt(i);
			--i;
		}
	}
	return commands;
}


/**
 *	@brief Overloaded functions: Filters the List of commands (search) by the given category
 *	
 *	Calls the filterByCategory function, translating the commandtype first
 *	
 *	@author Peter Grasch
 *	@param QString commandtype
 *	commandtype (Programme, Orte, Sonderzeichen)
 *	@param CommandList *commlist
 *	If supplied it contains the list to be filtered, if not the list is fetched from the Concept layer
 */
CommandList RunApplicationView::filterByStrCategory(QString commandtype, CommandList *commlist)
{
	return filterByCategory( getCategory( commandtype ), commlist );
}

/**
 *	@brief Translates a category-String to an int value
 *	
 *	0=Programme
 *	1=Orte
 *	2=Sonderzeichen
 *	3=Alles
 *	
 *	@author Peter Grasch
 *	@param QString commandtype
 *	commandtype 
 *	@return int
 *	The commandtype - translated
 */
int RunApplicationView::getCategory(QString commandtype)
{
	if (commandtype=="Programme")
		return 0;
	else if (commandtype == "Orte")
		return 1;
	else if (commandtype == "Sonderzeichen")
		return 2;
	else return 3; //most probably "Alles" - but you never know ^^ (and we need a default)
}


/**
 *	@brief Gets the selected QTableWidgetItem and passes it to runCommand(...)
 *	
 *	@author Peter Grasch
 */
void RunApplicationView::runSelectedCommand()
{
	if ( ui.twPrograms->currentItem() )
		this->runCommand( ui.twPrograms->currentItem() );
}

/**
 *	@brief inserts the given Commands in the QTableWidget
 *
 *	@author Peter Grasch, Susanne Tschernegg
 */
void RunApplicationView::insertCommands(CommandList list)
{
	int i=0;
	int type;
	QString strtype;
	ui.twPrograms->setRowCount(list.count());
    //ui.twPrograms->setItem(i,0, new QTableWidgetItem(QIcon(list.at(i)->getIconPath()),""));
	
    for (i=0; i<list.count(); i++)
	{
        QString resourceId = list.at(i)->getIconPath();
        
        QIcon icon;
        if(resourceId.contains(QRegExp(".dll,\n*")))
        {
            QStringList iconResources = resourceId.split(",");
            WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
            icon = windowsResourceHandler->retrieveIcon(iconResources.at(0), iconResources.at(1).toInt());
        }
        else
        {
            QPixmap pixmap(resourceId);
            icon.addPixmap(pixmap);
        }
        
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setIcon(icon);
        
        ui.twPrograms->setItem(i, 0, item);
        
		ui.twPrograms->setItem(i, 1, new QTableWidgetItem(list.at(i)->getName()));
		
		type = list.at(i)->getType();
		switch (type)
		{
			case 0:
				strtype = "Ausführen";
				break;
			case 1:
				strtype = "Ort";
				break;
			case 2:
				strtype = "Sonderzeichen";
				break;
		}
		
		ui.twPrograms->setItem(i, 2, new QTableWidgetItem(strtype));
		ui.twPrograms->setItem(i, 3, new QTableWidgetItem(list.at(i)->getValue()));
        ui.twPrograms->setItem(i, 4, new QTableWidgetItem(list.at(i)->getWorkingDirectory()));
		for (int j = 1; j<5; j++)
			ui.twPrograms->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
}

/**
*   \brief slot: the signal will be emited in the commandSettings.cpp
*           it updates the whole commandList
*
*   @author Susanne Tschernegg
*/
void RunApplicationView::loadCommands()
{
    ui.twPrograms->clearContents();
    run = new RunCommand();
    QString path = Settings::get("PathToCommands").toString();
    run->readCommands(path);
    insertCommands (run->getCommands());
}

/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunApplicationView::~RunApplicationView()
{
}



