//
// C++ Implementation: commandsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "commandsettings.h"
#include "command.h"
#ifdef __WIN32
#include <windows.h>
#endif
#include "importprogramwizard.h"
#include "logger.h"
#include "settings.h"

CommandSettings::CommandSettings(QWidget* parent): SystemWidget(tr("Kommandos"), QIcon(":/images/icons/emblem-system.svg"), tr("Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten"), parent)
{
    ui.setupUi(this);

    importProgramWizard = new ImportProgramWizard(this);
    
    help = tr("Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten");
    
    //connects
    //connect ( ui.twCommand, SIGNAL (itemChanged(QTableWidgetItem*)), this, SLOT(save()));
	connect ( ui.pbNewCommand, SIGNAL (clicked()), this, SLOT (newCommand()));
    connect ( ui.twCommand, SIGNAL (currentCellChanged(int,int,int,int) ), this, SLOT(checkAndAddCommandValues(int,int,int,int)));
	connect ( ui.pbDeleteCommand, SIGNAL (clicked()), this, SLOT (deleteCommand()));
	connect ( ui.twCommand, SIGNAL (cellDoubleClicked (int, int) ), this, SLOT(editCommand()));
	connect ( ui.pbEditCommand, SIGNAL (clicked()), this, SLOT (editCommand()));
	//connect ( ui.pbReloadCommands, SIGNAL (clicked()), this, SLOT (reset()));
	connect ( ui.cbShowCommand, SIGNAL (currentIndexChanged ( const QString &) ), this, SLOT(showOnlyCommands()));
	connect ( ui.pbClearSearchCommand, SIGNAL(clicked()), this, SLOT(clearSearchLineEdit()));
	connect ( ui.leSearchCommand, SIGNAL(textChanged(const QString &)), this, SLOT(searchCommandList()));
    connect ( ui.pbImportProgram, SIGNAL(clicked()), this, SLOT(importNewProgram()));
	ui.twCommand->resizeColumnToContents(1);
    commandLoader = new XMLCommand("conf/commands.xml");
    connect(importProgramWizard, SIGNAL(commandCreated(Command*)), this, SLOT(insertCommand(Command*)));
   // QMessageBox::information(0, "commandsettings","constructor");
   
}


CommandSettings::~CommandSettings()
{
}

bool CommandSettings::apply()
{
	Logger::log(tr("[INF] Speichere Kommandos..."));
    deactivateAllCbs();

    QString type;
    Command *newCommand;
    for(int i=0; i<ui.twCommand->rowCount(); i++)
    {
        type = ui.twCommand->item(i,1)->text();
        int typeInt;
        if(type == "Orte")
            typeInt = 1;
        else if (type == "Sonderzeichen")
            typeInt = 2;
        else typeInt = 0;

        newCommand = new Command(ui.twCommand->item(i,0)->text(), CommandType(typeInt), ui.twCommand->item(i,2)->text());
        if(commandLoader->commandExists(ui.twCommand->item(i,0)->data(Qt::UserRole).toString()))
        {
            commandLoader->replaceCommand(ui.twCommand->item(i,0)->data(Qt::UserRole).toString(), newCommand);
        }
        else
        {
            commandLoader->addCommand(newCommand);;
        }
        if(ui.twCommand->item(i,0)->data(Qt::UserRole).toString()==NULL)
            ui.twCommand->item(i,0)->setData(Qt::UserRole, ui.twCommand->item(i,0)->text());
    }
    commandsCount = ui.twCommand->rowCount();
	bool success = commandLoader->save(Settings::get("PathToCommands").toString());
    emit commandsChanged();
    return success;
}

/**
 * \brief Loads the commands from the corresponding XML file and inserts them into the list for configuring the available comamnds
 * \author Peter Grasch, Tschernegg Susanne
 */
bool CommandSettings::init()
{
    QString path = Settings::get("PathToCommands").toString();
	Logger::log(tr("[INF] Importiere Kommandos von ")+path);
	if(!commandLoader->load(path)){ return false;}
    //return true;
	CommandList commands = commandLoader->getCommands();
	
	ui.twCommand->setRowCount(commands.count());
	QTableWidgetItem *tmp;
	
	//baue combobox
	
	Logger::log(tr("[INF] Habe ")+QString::number(commands.count())+tr("Kommandos gefunden"));
	for (int i=0; i < commands.count(); i++)
	{
		tmp = new QTableWidgetItem(commands.at(i)->getName());
        tmp->setData(Qt::UserRole, commands.at(i)->getName());
		ui.twCommand->setItem(i, 0, tmp);
		
		CommandType ctype = commands.at(i)->getType();
		QString strType;
			
		if (ctype ==place)
			strType = tr("Orte");
		else if (ctype == type)
			strType = tr("Sonderzeichen");
		else strType = tr("Programme");
		
		//tmp = new QTableWidgetItem(strType);
		
		//ui.twCommand->setItem(i, 1, tmp);
		
		//QComboBox *cbType = new QComboBox();
		//cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
		//cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
		//cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
		
		//cbType->setCurrentIndex(ctype);
		tmp = new QTableWidgetItem(strType);
		ui.twCommand->setItem(i, 1, tmp);
		//ui.twCommand->setCellWidget(i, 1, cbType);
		ui.twCommand->resizeColumnToContents(1);
		
		tmp = new QTableWidgetItem(commands.at(i)->getValue());
		ui.twCommand->setItem(i, 2, tmp);
	}
    commandsCount = commands.count();
    return true;
}

void CommandSettings::deactivateAllCbs()
{
    QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget(ui.twCommand->currentRow(), 1);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		ui.twCommand->removeCellWidget(ui.twCommand->currentRow(), 1);
		ui.twCommand->setItem(ui.twCommand->currentRow(), 1, tmp);
	}
	//commandLoader->save();    
}


/**
* \brief The TreeWidgetItem will be modified, to change the values. The type of a command can be chosen from a ComboBox.
*
*\author SusanneTschernegg
*/
void CommandSettings::editCommand()
{
	int row = ui.twCommand->currentRow();
	QString typeStr = ui.twCommand->item(row,1)->text();
	
	QComboBox *cbType = new QComboBox();
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
	
	int posCb;
	if(typeStr == "Orte")
		posCb = 1;
	else if (typeStr == "Sonderzeichen")
		posCb = 2;
	else posCb = 0;
	cbType->setCurrentIndex(posCb);
	ui.twCommand->setCellWidget(row, 1, cbType);

	//twCommand->selectColumn(0);
	//twCommand->selectRow(row);
	ui.twCommand->editItem(ui.twCommand->item(row, 0));
}

/**
* \brief a new line to set a new command will be created. The type of the command can be chosen from a combobox
*
* \author Susanne Tschernegg
*/
void CommandSettings::newCommand()
{
	int rows = ui.twCommand->rowCount();
	ui.twCommand->insertRow(rows);
	ui.twCommand->setCurrentCell(rows, 0);
	QTableWidgetItem *tmp = new QTableWidgetItem();
	ui.twCommand->setItem(rows, 0, tmp);
	ui.twCommand->editItem(tmp);
	QComboBox *cbType = new QComboBox(ui.twCommand);
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));

	ui.twCommand->setCellWidget(rows, 1, cbType);
	
	ui.twCommand->setItem(rows, 2, new QTableWidgetItem());
}

/**
* \brief a command will be deleted
*
* \author Susanne Tschernegg
*/
void CommandSettings::deleteCommand()
{
	commandLoader->deleteCommand(ui.twCommand->item(ui.twCommand->currentRow(), 0)->text());
	ui.twCommand->removeRow(ui.twCommand->currentRow());
}

/**
* \brief to activate the combobox with different types of a command
*
* \author Susanne Tschernegg
*/
void CommandSettings::activateCb()
{
	QComboBox *cbType = new QComboBox();
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
	ui.twCommand->setCellWidget(ui.twCommand->currentRow(), 1, cbType);
}

/**
* \brief writes the type - which was chosen bevor in the combobox - in plaintext out
*
* \author Susanne Tschernegg
*/
void CommandSettings::checkAndAddCommandValues(int currRow, int currCol, int prevRow, int prevCol)
{
    //if no prevRow exists (no row was selected bevor)
    if(prevRow < 0)
        return;
    //QMessageBox::information(this,"preRow", QString::number(prevRow));
    //QMessageBox::information(this,"prevCol", QString::number(prevCol));
    
    //if the column name was selected
    if((prevCol==0))// && (ui.twCommand->item(prevRow, 0)->text()!=NULL) && ((ui.twCommand->item(prevRow,0)->text().trimmed()!="")))
    {
        if(commandNameExists(ui.twCommand->item(prevRow, 0)->text(),prevRow))
        {
            //if the name exists
             return;
        }
    }

    //if the column value was selected
    else if((prevCol==2))// && (ui.twCommand->item(prevRow, 2)->text()!=NULL) && ((ui.twCommand->item(prevRow,2)->text().trimmed()!="")))
    {
        if(commandValueExists(ui.twCommand->item(prevRow, 2)->text(), prevRow))
        {
            //if the command has been deleted
            return;
        }
    }
    
    //if the row wasn't changed
	if(currRow == prevRow)
    {
		return;
    }
    
    //if the commandvalues weren't set at all
    if(!allCommandValuesSet(prevRow))
        return;
	
    addCommand(prevRow);
}

/**
* \brief reloads the commands
*
* \author Susanne Tschernegg
*/
bool CommandSettings::reset()
{
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
    return init();
}

/**
* \brief shows only the commands, which have the selected type of the combobox cbShowCommand
*
* \author Susanne Tschernegg
*/
void CommandSettings::showOnlyCommands()
{
	QString currType = ui.cbShowCommand->currentText();
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
	QTableWidgetItem *tmp;
	CommandList commands = commandLoader->getCommands();
	int counter = 0;
	for (int i=0; i < commands.count(); i++)
	{
		CommandType ctype = commands.at(i)->getType();
		QString strType;
			
		if (ctype ==place)
			strType = tr("Orte");
		else if (ctype == type)
			strType = tr("Sonderzeichen");
		else strType = tr("Programme");
        
		if((strType==currType)||(currType=="Alles"))
		{
			counter ++;
			ui.twCommand->setRowCount(counter);
			
			tmp = new QTableWidgetItem(commands.at(i)->getName());
			ui.twCommand->setItem(counter-1, 0, tmp);
			
			tmp = new QTableWidgetItem(strType);
			ui.twCommand->setItem(counter-1, 1, tmp);
			
			tmp = new QTableWidgetItem(commands.at(i)->getValue());
			ui.twCommand->setItem(counter-1, 2, tmp);
		}
	}
	ui.twCommand->resizeColumnToContents(1);
}

/**
* \brief clears the lineedit lesearchcommand
*
* \author Susanne Tschernegg
*/
void CommandSettings::clearSearchLineEdit()
{
	ui.leSearchCommand->clear();
}

/**
* \brief searches for commands which includes the text and the chosen type
*
* \author Susanne Tschernegg
*/
void CommandSettings::searchCommandList()
{
	QString searchText = ui.leSearchCommand->text();
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
	QTableWidgetItem *tmp;
	CommandList commands = commandLoader->getCommands();
	int counter = 0;
	for (int i=0; i < commands.count(); i++)
	{	
		QString name = commands.at(i)->getName();
		if(name.contains(searchText,Qt::CaseInsensitive))
		{
			counter ++;
			ui.twCommand->setRowCount(counter);
			
			tmp = new QTableWidgetItem(commands.at(i)->getName());
			ui.twCommand->setItem(counter-1, 0, tmp);
			
			CommandType ctype = commands.at(i)->getType();
			QString strType;
				
			if (ctype ==place)
				strType = tr("Orte");
			else if (ctype == type)
				strType = tr("Sonderzeichen");
			else strType = tr("Programme");
			
			tmp = new QTableWidgetItem(strType);
			ui.twCommand->setItem(counter-1, 1, tmp);
			
			tmp = new QTableWidgetItem(commands.at(i)->getValue());
			ui.twCommand->setItem(counter-1, 2, tmp);
		}
	}
	ui.twCommand->resizeColumnToContents(1);
}

/**
* \brief to test the importprogramwizard
*
* \author Susanne Tschernegg
*/
void CommandSettings::importNewProgram()
{
    importProgramWizard->show();
}


void CommandSettings::insertCommand(Command* command)
{
	int rows = ui.twCommand->rowCount();
	ui.twCommand->insertRow(rows);
    
    QTableWidgetItem *tmp = new QTableWidgetItem(command->getName());
    tmp->setData(Qt::UserRole, command->getName());
	ui.twCommand->setItem(rows, 0, tmp);
    
    CommandType ctype = command->getType();
    QString strType;
    if (ctype ==place)
        strType = tr("Orte");
    else if (ctype == type)
        strType = tr("Sonderzeichen");
    else strType = tr("Programme");
    ui.twCommand->setItem(rows, 1, new	QTableWidgetItem(strType));
    
	ui.twCommand->setItem(rows, 2, new QTableWidgetItem(command->getValue()));


    for(int i=0; i<ui.twCommand->rowCount()-1; i++)
    {
        if((i!=rows) && (QString::compare(command->getName(), ui.twCommand->item(i, 0)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::information(this, tr("Name existiert bereits"), tr("Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte ändern Sie in der Kommandoliste den bereits bestehenden Namen."),
                QMessageBox::Ok | QMessageBox::Cancel);
            
            commandLoader->deleteCommand(ui.twCommand->item(rows, 0)->text());
            ui.twCommand->removeRow(rows);
        }
    }

 /*   Command *newCommand = new Command(ui.twCommand->item(prevRow,0)->text(), CommandType(typeInt), ui.twCommand->item(prevRow,2)->text());
    if(commandLoader->commandExists((ui.twCommand->item(prevRow,0)->data(Qt::UserRole).toString()))
        commandLoader->replaceCommand(ui.twCommand->item(prevRow,0)->text(), newCommand);
    else commandLoader->addCommand(newCommand);*/
	//}
}


//row = prevRow
bool CommandSettings::commandNameExists(QString name, int prevRow)
{
    for(int i=0; i<ui.twCommand->rowCount()-1; i++)
    {
        if((i!=prevRow) && (QString::compare(name, ui.twCommand->item(i, 0)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::information(this, tr("Name existiert bereits"), tr("Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte geben Sie einen anderen Namen für dieses Kommando ein.\n\nWollen Sie das Kommando löschen? (Klickes Sie Nein, um den Namen jetzt zu ändern)"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes)
             {
                 if(ui.twCommand->item(prevRow, 0)->data(Qt::UserRole)!=NULL)
                    commandLoader->deleteCommand(ui.twCommand->item(prevRow, 0)->data(Qt::UserRole).toString());
                 else commandLoader->deleteCommand(ui.twCommand->item(prevRow, 0)->text());
                ui.twCommand->removeRow(prevRow);
            }
             else
             {
                ui.twCommand->selectColumn(0);
                ui.twCommand->selectRow(prevRow);
                ui.twCommand->editItem(ui.twCommand->item(prevRow, 0));
             }
            return true;
        }
    }
    return false;
}

bool CommandSettings::commandValueExists(QString value, int prevRow)
{
    for(int i=0; i<ui.twCommand->rowCount()-1; i++)
    {
        if((i!=prevRow) && (QString::compare(value, ui.twCommand->item(i, 2)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::critical(this, tr("Name existiert bereits"), tr("Dieser Wert existiert bereits!\n\nWollen Sie den Wert jetzt ändern? (Klickes Sie Nein, um den Wert jetzt zu behalten)"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes) 
             {
                //ui.twCommand->selectColumn(2);
                //ui.twCommand->selectRow(prevRow);
                ui.twCommand->editItem(ui.twCommand->item(prevRow, 2));
                return true;
             }
            //return true;
        }
    }
    return false;
}

bool CommandSettings::allCommandValuesSet(int prevRow)
{
    if((ui.twCommand->item(prevRow, 0)->text()==NULL)||(ui.twCommand->item(prevRow, 0)->text()==NULL)||
		(ui.twCommand->item(prevRow,0)->text().trimmed()=="")||(ui.twCommand->item(prevRow,2)->text().trimmed()==""))
	{
		int result = QMessageBox::question(this, tr("Leeres Kommandofeld"), tr("Dieses Kommando wurde nicht vollständig ausgefüllt.\nJedes Kommando muss einen Namen, Wert und Typ besitzen.\n\nWollen Sie diesen Eintrag jetzt löschen? (Klicken Sie Nein, um das Kommando jetzt zu vervollständigen)"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		 if (result == QMessageBox::Yes) 
		 {
             if(ui.twCommand->item(prevRow, 0)->data(Qt::UserRole)!=NULL)
                commandLoader->deleteCommand(ui.twCommand->item(prevRow, 0)->data(Qt::UserRole).toString());
             else commandLoader->deleteCommand(ui.twCommand->item(prevRow, 0)->text());
            ui.twCommand->removeRow(prevRow);
		 } else
		 {
			ui.twCommand->selectColumn(0);
			ui.twCommand->selectRow(prevRow);
			ui.twCommand->editItem(ui.twCommand->item(prevRow, 0));
		 }
		 return false;
	 }
}

void CommandSettings::addCommand(int prevRow)
{
    QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget(prevRow, 1);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		ui.twCommand->removeCellWidget(prevRow, 1);
		ui.twCommand->setItem(prevRow, 1, tmp);
		
		int typeInt;
		if(type == "Orte")
			typeInt = 1;
		else if (type == "Sonderzeichen")
			typeInt = 2;
		else typeInt = 0;

/*		Command *newCommand = new Command(ui.twCommand->item(prevRow,0)->text(), CommandType(typeInt), ui.twCommand->item(prevRow,2)->text());
		if(commandLoader->commandExists(ui.twCommand->item(prevRow,0)->text()))
			commandLoader->replaceCommand(ui.twCommand->item(prevRow,0)->text(), newCommand);
		else commandLoader->addCommand(newCommand);
            
        
        newCommand = new Command(ui.twCommand->item(i,0)->text(), CommandType(typeInt), ui.twCommand->item(i,2)->text());
        if(commandLoader->commandExists(ui.twCommand->item(i,0)->data(Qt::UserRole).toString()))
            commandLoader->replaceCommand(ui.twCommand->item(i,0)->text(), newCommand);
        else commandLoader->addCommand(newCommand);
        if(ui.twCommand->item(i,0)->data(Qt::UserRole).toString()==NULL)
            ui.twCommand->item(i,0)->setData(Qt::UserRole, ui.twCommand->item(i,0)->text());*/
	}
}

