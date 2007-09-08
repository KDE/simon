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
#include "importplacewizard.h"
#include "logger.h"
#include "settings.h"

/**
*   \brief constructor
*   @autor Susanne Tschernegg
*/
CommandSettings::CommandSettings(QWidget* parent): SystemWidget(tr("Kommandos"), QIcon(":/images/icons/emblem-system.svg"), tr("Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten"), parent)
{
    ui.setupUi(this);
    twCommand = new ReturnTableWidget(this);
    twCommand->setColumnCount(3);
    QStringList headerLabels;
    headerLabels.append(tr("Name"));
    headerLabels.append(tr("Typ"));
    headerLabels.append(tr("Wert"));
    twCommand->setHorizontalHeaderLabels(headerLabels);
    
    ui.vboxLayout->insertWidget(1, twCommand);
    
    ui.pbImportProgram->setCheckable(true);
    ui.pbImportPlace->setCheckable(true);
    importProgramWizard = new ImportProgramWizard(this);
    importPlaceWizard = new ImportPlaceWizard(this);
    
    help = tr("Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten");
    
    //connects
	connect ( ui.pbNewCommand, SIGNAL (clicked()), this, SLOT (newCommand()));
    connect ( twCommand, SIGNAL (currentCellChanged(int,int,int,int) ), this, SLOT(checkAndAddCommandValues(int,int,int,int)));
	connect ( ui.pbDeleteCommand, SIGNAL (clicked()), this, SLOT (deleteCommand()));
	connect ( twCommand, SIGNAL (cellDoubleClicked (int, int) ), this, SLOT(editCommand(int, int)));
	connect ( ui.pbEditCommand, SIGNAL (clicked()), this, SLOT (editCommand()));
	connect ( ui.cbShowCommand, SIGNAL (currentIndexChanged ( const QString &) ), this, SLOT(showOnlyCommands()));
	connect ( ui.pbClearSearchCommand, SIGNAL(clicked()), this, SLOT(clearSearchLineEdit()));
	connect ( ui.leSearchCommand, SIGNAL(textChanged(const QString &)), this, SLOT(searchCommandList()));
    connect ( ui.pbImportProgram, SIGNAL(clicked()), this, SLOT(importNewProgram()));
	twCommand->resizeColumnToContents(1);
    commandLoader = new XMLCommand("conf/commands.xml");
    connect(importProgramWizard, SIGNAL(commandCreated(Command*)), this, SLOT(insertCommand(Command*)));   
    connect(importProgramWizard, SIGNAL(finished(int)), this, SLOT(setWidgetsDisabled()));
    connect(this, SIGNAL(changeExistingName(bool)), importProgramWizard, SLOT(changeName(bool)));
    connect(twCommand, SIGNAL(returnPressed()), this, SLOT(checkValuesAfterReturnPressed()));
    connect(ui.leSchluesselWort, SIGNAL(textChanged(const QString &)),this,SLOT(checkValuesAfterReturnPressed()));
    connect(ui.pbClearSearchCommand, SIGNAL(clicked()),this,SLOT(checkValuesAfterReturnPressed()));
    connect(ui.pbImportPlace, SIGNAL(clicked()), this, SLOT(importNewPlace()));
}

/**
*   \brief destructor
*   @autor Susanne Tschernegg
*/
CommandSettings::~CommandSettings()
{
}

/**
*   \brief saves all commands. New created commands will be appended to the commandlist and existing commands will be updated.
*   @autor Susanne Tschernegg
*/
bool CommandSettings::apply()
{
   // QMessageBox::information(this, "commandsettings", "apply");
	Logger::log(tr("[INF] Speichere Kommandos..."));
    
    int currRow;
    if(twCommand->currentRow())
        currRow = twCommand->currentRow();
    else currRow = -1;
   // QString str = twCommand->item(currRow, 0)->text();
 /*   if(currRow>=0)
    {
        if(!allCommandValuesSet(currRow))
        {
            return false;
        }
        if(commandEdited)
        {
            if(commandNameExists(ui.twCommand->item(currRow, 0)->text(), currRow))
            {
                return false;
            }
            if(commandValueExists(ui.twCommand->item(currRow, 2)->text(), currRow))
            {
                return false;
            }
        }
        deactivateAllCbs();
    }*/
    if(commandEdited && currRow>=0)
    {
        //int currRow = twCommand->currentRow();
        int currCol = twCommand->currentColumn();
        twCommand->setCurrentCell(0,0);
        //commandEdited = false;
        if(currRow<twCommand->rowCount())
            twCommand->setCurrentCell(currRow,currCol);
    }
    
    ui.leSearchCommand->clear();
    showAllCommands();
    
/*    if(twCommand->rowCount()<1)
        return false;*/ //13:41
    
//    if(currRow==twCommand->rowCount()-1)
//        return false;
    //else if(ui.twCommand->item(currRow,0)==str)
    
  //  ui.leSearchCommand->clear();
  //  showAllCommands();

    commandEdited=false;
    QString type;
    Command *newCommand;
    for(int i=0; i<twCommand->rowCount(); i++)
    {
        type = twCommand->item(i,1)->text();
        int typeInt;
        if(type == "Orte")
            typeInt = 1;
        else if (type == "Sonderzeichen")
            typeInt = 2;
        else typeInt = 0;

        //creates a new command or replaces it
        newCommand = new Command(twCommand->item(i,0)->text(), CommandType(typeInt), twCommand->item(i,2)->text());
        
        if((!twCommand->item(i,0)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(i,0)->data(Qt::UserRole).toString())))
        {
            //replaces the old command
        //    QMessageBox::information(this,"apply ... replace", twCommand->item(i,0)->data(Qt::UserRole).toString());
            commandLoader->replaceCommand(twCommand->item(i,0)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(i,0)->setData(Qt::UserRole, twCommand->item(i,0)->text());
        }
        else
        {
            //creates a new command
       //     QMessageBox::information(this,"apply ... add", twCommand->item(i,2)->text());
         //   QMessageBox::information(this,"apply ... add", twCommand->item(i,0)->data(Qt::UserRole).toString());
            commandLoader->addCommand(newCommand);
            twCommand->item(i,0)->setData(Qt::UserRole, twCommand->item(i,0)->text());
        }         
    }
    
    commandsCount = twCommand->rowCount();
	bool success = commandLoader->save(Settings::get("PathToCommands").toString());
    emit commandsChanged();
    commandEdited = false;
    return success;
}

/**
 * \brief Loads the commands from the corresponding XML file and inserts them into the list for configuring the available comamnds
 * \author Peter Grasch, Tschernegg Susanne
 */
bool CommandSettings::init()
{
    //QMessageBox::information(this, "commandsettings", "init");
    QString path = Settings::get("PathToCommands").toString();
	Logger::log(tr("[INF] Importiere Kommandos von ")+path);
	if(!commandLoader->load(path)){ return false;}
	CommandList commands = commandLoader->getCommands();
	
	twCommand->setRowCount(commands.count());
	QTableWidgetItem *tmp;
	
    ui.cbShowCommand->setCurrentIndex(0);
    ui.leSearchCommand->clear();
	
	Logger::log(tr("[INF] Habe ")+QString::number(commands.count())+tr("Kommandos gefunden"));
	for (int i=0; i < commands.count(); i++)
	{
		tmp = new QTableWidgetItem(commands.at(i)->getName());
        tmp->setData(Qt::UserRole, commands.at(i)->getName());
		twCommand->setItem(i, 0, tmp);
		
		CommandType ctype = commands.at(i)->getType();
		QString strType;
			
		if (ctype ==place)
			strType = tr("Orte");
		else if (ctype == type)
			strType = tr("Sonderzeichen");
		else strType = tr("Programme");
		
		tmp = new QTableWidgetItem(strType);
		twCommand->setItem(i, 1, tmp);
		twCommand->resizeColumnToContents(1);
		
		tmp = new QTableWidgetItem(commands.at(i)->getValue());
		twCommand->setItem(i, 2, tmp);
	}
    commandsCount = commands.count();
   // if(commandsCount>0)
     //   twCommand->setCurrentCell(0,0);
    //connect ( ui.twCommand, SIGNAL (itemChanged(QTableWidgetItem *)), this, SLOT(CommandValues()));
    //connect ( ui.twCommand, SIGNAL (cellChanged(int, int)), this, SLOT(checkAndAddCommandValues()));
    return true;
}

/**
*   \brief if any commandline has still a combobox to choose a type for the command, the combobox will be deleted and
        writes the type as plain text instead in the cell.
*   @autor Susanne Tschernegg
*/
void CommandSettings::deactivateAllCbs()
{
    //QMessageBox::information(this, "commandsettings", "deactivateAllCbs");
    QWidget *tmpWidget = new QWidget();
	tmpWidget = twCommand->cellWidget(twCommand->currentRow(), 1);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		twCommand->removeCellWidget(twCommand->currentRow(), 1);
		twCommand->setItem(twCommand->currentRow(), 1, tmp);
        
        int typeInt;
		if(type == "Orte")
			typeInt = 1;
		else if (type == "Sonderzeichen")
			typeInt = 2;
		else typeInt = 0;
        int currRow = twCommand->currentRow();

        //creates a new command resp. replaces it
        Command *newCommand = new Command(twCommand->item(currRow,0)->text(), CommandType(typeInt), twCommand->item(currRow,2)->text());
        if((!twCommand->item(currRow,0)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(currRow,0)->data(Qt::UserRole).toString())))
        {
            //replaces the old command
            commandLoader->replaceCommand(twCommand->item(currRow,0)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(currRow,0)->setData(Qt::UserRole, twCommand->item(currRow,0)->text());
        }
        else
        {
            commandLoader->addCommand(newCommand);
            twCommand->item(currRow,0)->setData(Qt::UserRole, twCommand->item(currRow,0)->text());
        }
    }
}


/**
* \brief The TreeWidgetItem will be modified, to change the values. The type of a command can be chosen from a ComboBox.
*
*\author SusanneTschernegg
*/
void CommandSettings::editCommand(int row, int column)
{
    //QMessageBox::information(this, "commandsettings", "editCommand");
    if(twCommand->currentRow()<0)
        return;
    commandEdited = true;
    if(row<0)
	    row = twCommand->currentRow();
    
    QWidget *tmpWidget = new QWidget();
	tmpWidget = twCommand->cellWidget(row, 1);
	if(tmpWidget==NULL)
	{
	    QString typeStr = twCommand->item(row,1)->text();
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
        twCommand->setCellWidget(row, 1, cbType);
    }
    //twCommand->selectColumn(0);
    //twCommand->selectRow(row);
    twCommand->setCurrentCell(row,column);
    twCommand->editItem(twCommand->item(row, column));
}

/**
* \brief a new line to set a new command will be created. The type of the command can be chosen from a combobox
*
* \author Susanne Tschernegg
*/
void CommandSettings::newCommand()
{
    //QMessageBox::information(this, "commandsettings", "newCommand");
  //  int currRow = -1;
   // if(twCommand->currentRow())
    if(commandEdited)
    {
        //twCommand->closePersistentEditor(twCommand->currentItem());
        if(twCommand->currentRow()!=0)
            twCommand->setCurrentCell(0,0);
        else if(twCommand->rowCount()>1)
            twCommand->setCurrentCell(1,0);
    }
    
    //int currRow = twCommand->currentRow();
    //    QMessageBox::information(this,"currRow", QString::number(currRow));
 /*   if(currRow>=0 && commandEdited) //11:22
    {
        if(!allCommandValuesSet(currRow) && commandEdited)  //11:15 commandEdited
        {
            QMessageBox::information(this,"commandsettings","im allcommandvaluesset");
            //ui.pbImportProgram->setChecked(false);
            commandEdited=true;
            return;
        }
        if(commandEdited)
        {
            QMessageBox::information(this,"commandsettings","commandedited = TRUE");
            if(commandNameExists(twCommand->item(currRow, 0)->text(), currRow))
            {
                QMessageBox::information(this,"commandsettings","im commandnameexists");
              //  ui.pbImportProgram->setChecked(false);
                commandEdited=true;
                return;
            }
            if(commandValueExists(twCommand->item(currRow, 2)->text(), currRow))
            {
                QMessageBox::information(this,"commandsettings","im commandvalueexists");
                //ui.pbImportProgram->setChecked(false);
                commandEdited=true;
                return;
            }
        }
         QMessageBox::information(this,"commandsettings","vor deactivateallcbs");
        deactivateAllCbs();
    }*/ //11:21
    //twCommand->setCurrentCell(0,0);
    if(commandEdited)
        return;
        //QMessageBox::information(this,"commandsettings","nach da if schleife");
    ui.leSearchCommand->clear();
    showAllCommands();
	int rows = twCommand->rowCount();
    twCommand->setRowCount(rows);
	twCommand->insertRow(rows);
	QTableWidgetItem *tmp = new QTableWidgetItem();
	twCommand->setItem(rows, 0, tmp);
//	twCommand->editItem(tmp);   //09:58
        //QMessageBox::information(this,"warum geht des net","nach edititem(tmp)");
	QComboBox *cbType = new QComboBox(twCommand);
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
	twCommand->setCellWidget(rows, 1, cbType);
	twCommand->setItem(rows, 2, new QTableWidgetItem());
    //if(twCommand->currentRow()<0)
       // twCommand->setCurrentCell(rows, 0);
    //twCommand->selectColumn(0);
    //if(twCommand->currentRow()<0)
        //QMessageBox::information(this,"warum geht des net","nach edititem(tmp)2 vor 2");
//    twCommand->selectRow(rows);     //09:15, 09:59
        //QMessageBox::information(this,"warum geht des net","nach edititem(tmp)2");
    //twCommand->setCurrentCell(rows, 0);
    //twCommand->setCurrentRow(rows);
    //twCommand->setCurrentItem(tmp);
        //QMessageBox::information(this,"warum geht des net","nach edititem(tmp)3");
    twCommand->setCurrentCell(rows,0);  //10:33
    twCommand->editItem(twCommand->item(rows, 0));
    commandEdited = true;
}

/**
* \brief a command will be deleted
*
* \author Susanne Tschernegg
*/
void CommandSettings::deleteCommand()
{
    if(twCommand->currentRow()<0)
        return;
    int rowCount = twCommand->rowCount();
    //QMessageBox::information(this, "commandsettings", "deleteCommand");
    int currRow = twCommand->currentRow();
  //  QMessageBox::information(this,"currRow",QString::number(currRow));
 //   QString data = twCommand->item(currRow,0)->getData(Qt::UserRole).toString();  //14:12
    
    if(currRow!=0)
        twCommand->setCurrentCell(0,0);
    else if(twCommand->rowCount()>1)
        twCommand->setCurrentCell(1,0);  //14:09
   // QMessageBox::information(this,"ajdlf","ajgjapewji");
    //QMessageBox::information(this,"currRow",QString::number(currRow));
    
    if(commandEdited)
    {
        //commandEdited=false;
        return;
    }
    
  /*  if((ui.twCommand->item(currRow, 0)->text()==NULL)||(ui.twCommand->item(currRow, 0)->text()==NULL)||
        (ui.twCommand->item(currRow,0)->text().trimmed()=="")||(ui.twCommand->item(currRow,2)->text().trimmed()==""))
    {
        commandEdited = true;
        return;
    }*/
 /*   if(!twCommand->item(currRow,0))
        return;
    if((twCommand->item(currRow, 0)->text()==NULL)||(twCommand->item(currRow, 0)->text()==NULL)||
        (twCommand->item(currRow,0)->text().trimmed()=="")||(twCommand->item(currRow,2)->text().trimmed()==""))
    {
        commandEdited = true;
        return;
    }*/ //14:09
    
    if(rowCount==twCommand->rowCount())
    {
        if(!twCommand->item(currRow,0)->data(Qt::UserRole).isNull())
            commandLoader->deleteCommand(twCommand->item(currRow, 0)->data(Qt::UserRole).toString());//, ui.twCommand->item(currRow, 2)->text());
        twCommand->removeRow(currRow);
    }
}

/**
* \brief to activate the combobox with different types of a command
*
* \author Susanne Tschernegg
*/
void CommandSettings::activateCb()
{
    //QMessageBox::information(this, "commandsettings", "activateCb");
	QComboBox *cbType = new QComboBox();
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
	twCommand->setCellWidget(twCommand->currentRow(), 1, cbType);
}

/**
* \brief writes the type - which was chosen bevor in the combobox - in plaintext out
*
* \author Susanne Tschernegg
*/
void CommandSettings::checkAndAddCommandValues(int currRow, int currCol, int prevRow, int prevCol)
{
  //  QMessageBox::information(this, "commandsettings", "checkAndAddCommandValues");
    //if no prevRow exists (no row was selected bevor)
    //bool onlyItemChanged = false;
    //if(currRow==0 && currCol==0 && prevRow==0 && prevCol==0)
     //   onlyItemChanged = true;
    if(prevRow < 0)
        return;
    //QMessageBox::information(this,"prevrow mmmmmmmmmmmm", QString::number(prevRow));
       // prevRow = ui.twCommand->currentRow();
    //if(onlyItemChanged)
    //{
    //    prevRow = ui.twCommand->currentRow();
    //    prevCol = ui.twCommand->currentColumn();
    //    QMessageBox::information(this,"prevRow", QString::number(prevRow));
    //    QMessageBox::information(this,"prevCol", QString::number(prevCol));
    //}
    //if the column name was selected
        //QMessageBox::information(this, "checkAndAdd prevRow",QString::number(prevRow));
        //QMessageBox::information(this,"checkAndAdd curRow",QString::number(currRow));
    if((prevCol==0) && (commandEdited))// && (ui.twCommand->item(prevRow, 0)->text()!=NULL) && ((ui.twCommand->item(prevRow,0)->text().trimmed()!="")))
    {
            //QMessageBox::information(this,"commandsettings","vor commandnameexists");
        if(commandNameExists(twCommand->item(prevRow, 0)->text(),prevRow))
        {
            //if the name exists
            ui.pbImportProgram->setChecked(false);
            commandEdited = true;
             return;
        }
    }
    
    //if the column value was selected
    else if((prevCol==2) && (commandEdited))// && (ui.twCommand->item(prevRow, 2)->text()!=NULL) && ((ui.twCommand->item(prevRow,2)->text().trimmed()!="")))
    {
        if(commandValueExists(twCommand->item(prevRow, 2)->text(), prevRow))
        {
            //if the command has been deleted
            ui.pbImportProgram->setChecked(false);
            commandEdited = true;
            return;
        }
    }
    
    //if the row hasn't been changed
	if((currRow == prevRow))//&&(!prevRow<0))//&&(onlyItemChanged))
    {
            //QMessageBox::information(this,"checkAndAdd","currRow == prevRow");
        //commandEdited = true;
		return;
    }
    
        //QMessageBox::information(this,"commandsettings","vor allcommandValuesset");
    //if the commandvalues weren't set at all
    if(!allCommandValuesSet(prevRow) && commandEdited)  //11:19
    {
            //QMessageBox::information(this,"commandsettings","im allcommandValuesset");
        ui.pbImportProgram->setChecked(false);
        commandEdited = true;
        return;
    }
        //QMessageBox::information(this,"commandsettings","nach allcommandValuesset");
    
    QString type;
 /*   if(commandEdited==false)
    for(int i=0; i<twCommand->rowCount(); i++)
    {
        QMessageBox::information(this,"commandsettings in da schleife",QString::number(i));
        type = twCommand->item(i,1)->text();
        int typeInt;
        if(type == "Orte")
            typeInt = 1;
        else if (type == "Sonderzeichen")
            typeInt = 2;
        else typeInt = 0;
    
        //creates esp. replaces a command
        Command *newCommand = new Command(twCommand->item(i,0)->text(), CommandType(typeInt), twCommand->item(i,2)->text());
        if((!twCommand->item(i,0)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(i,0)->data(Qt::UserRole).toString())))
        {
           //replaces the command
            commandLoader->replaceCommand(twCommand->item(i,0)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(i,0)->setData(Qt::UserRole, twCommand->item(i,0)->text());
        }
        else
        {
            commandLoader->addCommand(newCommand);
            twCommand->item(i,0)->setData(Qt::UserRole, twCommand->item(i,0)->text());
        }
    }*/
        //QMessageBox::information(this,"commandsettings","nach da for schleife");
    deactivateCB(prevRow);
    commandEdited = false;
}

/**
* \brief reloads the commands
*
* \author Susanne Tschernegg
*/
bool CommandSettings::reset()
{
    //QMessageBox::information(this, "commandsettings", "reset");
    disconnect ( twCommand, SIGNAL (itemChanged(QTableWidgetItem *)), this, SLOT(checkAndAddCommandValues()));
    int currRow = twCommand->currentRow();
    if(currRow>=0)
    {
        if(!allCommandValuesSet(currRow))
        {
            ui.pbImportProgram->setChecked(false);
            return false;
        }
        if(commandEdited)
        {
            if(commandNameExists(twCommand->item(currRow, 0)->text(), currRow))
            {
                ui.pbImportProgram->setChecked(false);
                return false;
            }
            if(commandValueExists(twCommand->item(currRow, 2)->text(), currRow))
            {
                ui.pbImportProgram->setChecked(false);
                return false;
            }
        }
        //deactivateAllCbs();
    }
	twCommand->clearContents();
	twCommand->setRowCount(0);
    return init();
}

/**
* \brief shows only the commands, which have the selected type of the combobox cbShowCommand
*
* \author Susanne Tschernegg
*/
void CommandSettings::showOnlyCommands()
{   
    if(commandEdited)   //14:42
    {
        if(ui.cbShowCommand->currentIndex()!=0)
        {
            ui.cbShowCommand->setCurrentIndex(0);   //14:43
            return;
        }
        
        if(!(twCommand->currentRow()<0))
        {
//             int rowCount = twCommand->rowCount();
            //QMessageBox::information(this, "commandsettings", "deleteCommand");
            int currRow = twCommand->currentRow();
          //  QMessageBox::information(this,"currRow",QString::number(currRow));
         //   QString data = twCommand->item(currRow,0)->getData(Qt::UserRole).toString();  //14:12
            
            if(currRow!=0)
                twCommand->setCurrentCell(0,0);
            else if(twCommand->rowCount()>1)
                twCommand->setCurrentCell(1,0);  //14:41
        }
        return;
    }
    //QMessageBox::information(this, "commandsettings", "showOnlyCommands");

    int currInd = ui.cbShowCommand->currentIndex();
    if(currInd!=0)
        ui.leSearchCommand->clear();
    ui.cbShowCommand->setCurrentIndex(currInd);
    int currRow = twCommand->currentRow();
    if(currRow>=0)
    {
        if(!allCommandValuesSet(currRow))
        {
            return;
        }
        if(commandEdited)
        {
            if(commandNameExists(twCommand->item(currRow, 0)->text(), currRow))
            {
                return;
            }
            if(commandValueExists(twCommand->item(currRow, 2)->text(), currRow))
            {
                return;
            }
        }
        deactivateAllCbs();
    }
    
	QString currType = ui.cbShowCommand->currentText();
	twCommand->clearContents();
	twCommand->setRowCount(0);
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
			twCommand->setRowCount(counter);
			
			tmp = new QTableWidgetItem(commands.at(i)->getName());
            tmp->setData(Qt::UserRole, commands.at(i)->getName());
			twCommand->setItem(counter-1, 0, tmp);
			
			tmp = new QTableWidgetItem(strType);
			twCommand->setItem(counter-1, 1, tmp);
			
			tmp = new QTableWidgetItem(commands.at(i)->getValue());
			twCommand->setItem(counter-1, 2, tmp);
		}
	}
	twCommand->resizeColumnToContents(1);
}

/**
* \brief clears the lineedit lesearchcommand
*
* \author Susanne Tschernegg
*/
void CommandSettings::clearSearchLineEdit()
{
    //QMessageBox::information(this, "commandsettings", "clearSearchLineEdit");
	ui.leSearchCommand->clear();
}

/**
* \brief searches for commands which includes the text and the chosen type
*
* \author Susanne Tschernegg
*/
void CommandSettings::searchCommandList()
{
    //QMessageBox::information(this, "commandsettings", "searchCommandList");
    //ui.cbShowCommand->setCurrentIndex(0);
    if((ui.leSearchCommand->text()=="") && (!commandEdited)) //16:39
    {
        //showAllCommands();  //e.g. search only for places
        CommandList allCommands = commandLoader->getCommands();
        twCommand->clearContents();
	    //twCommand->setRowCount(allCommands.count());
        twCommand->setRowCount(0);
	    QTableWidgetItem *tmp;
        int counter = 0;
        for(int z=0; z<allCommands.count(); z++)
        {
            CommandType ctype = allCommands.at(z)->getType();
			QString strType;
				
			if (ctype ==place)
				strType = tr("Orte");
			else if (ctype == type)
				strType = tr("Sonderzeichen");
			else strType = tr("Programme");
            
            
            if((ui.cbShowCommand->currentIndex()==0) || (strType==ui.cbShowCommand->currentText()))
            {
                counter++;
                twCommand->setRowCount(counter);
                
                tmp = new QTableWidgetItem(allCommands.at(z)->getName());
                tmp->setData(Qt::UserRole, allCommands.at(z)->getName());
                twCommand->setItem(counter-1, 0, tmp);
                
                CommandType ctype = allCommands.at(z)->getType();
                QString strType;
                    
                if (ctype ==place)
                    strType = tr("Orte");
                else if (ctype == type)
                    strType = tr("Sonderzeichen");
                else strType = tr("Programme");
                
                tmp = new QTableWidgetItem(strType);
                twCommand->setItem(counter-1, 1, tmp);
                
                tmp = new QTableWidgetItem(allCommands.at(z)->getValue());
                twCommand->setItem(counter-1, 2, tmp);
            }
        }
        twCommand->resizeColumnToContents(1);
        return;
    }
    
        //QMessageBox::information(this, "commandsettings", "searchCommandList11");
    int prevRow = twCommand->currentRow();
        //QMessageBox::information(this, "commandsettings currrow", QString::number(prevRow)); 
    if(ui.leSearchCommand->text()!="")
        if(twCommand->rowCount()>0 && prevRow!=0)    //08:53
            twCommand->setCurrentCell(0,0);
            //twCommand->selectRow(0);
        else if(twCommand->rowCount()>1)
            twCommand->setCurrentCell(1,0);
            //twCommand->selectRow(1);
    
/*    if(commandEdited) //08:46
    {
        commandEdited=false;
        return;
    }*/
    
        //QMessageBox::information(this, "commandsettings", "searchCommandList22");
    if(!twCommand->item(prevRow,0))
    {
        if(!prevRow>=twCommand->rowCount())
            return;
    }
    else
    {
        if((twCommand->item(prevRow, 0)->text()==NULL)||(twCommand->item(prevRow, 0)->text()==NULL)||
            (twCommand->item(prevRow,0)->text().trimmed()=="")||(twCommand->item(prevRow,2)->text().trimmed()==""))
        {
            commandEdited = true;
            ui.leSearchCommand->clear();
            return;
        }
    }

        //QMessageBox::information(this, "commandsettings", "searchCommandList33");
  //  showAllCommands();    //16:51
    
	QString searchText = ui.leSearchCommand->text();
	twCommand->clearContents();
	twCommand->setRowCount(0);
	QTableWidgetItem *tmp;
	CommandList commands = commandLoader->getCommands();
	int counter = 0;
	for (int i=0; i < commands.count(); i++)
	{
		QString name = commands.at(i)->getName();
		if(name.contains(searchText,Qt::CaseInsensitive))
		{
            CommandType ctype = commands.at(i)->getType();
            QString strType;
                
            if (ctype ==place)
                strType = tr("Orte");
            else if (ctype == type)
                strType = tr("Sonderzeichen");
            else strType = tr("Programme");
            if((ui.cbShowCommand->currentIndex()==0) || (strType==ui.cbShowCommand->currentText())) //16:52
            {
                counter ++;
                twCommand->setRowCount(counter);
                
                tmp = new QTableWidgetItem(commands.at(i)->getName());
                tmp->setData(Qt::UserRole, commands.at(i)->getName());
                twCommand->setItem(counter-1, 0, tmp);
                
                tmp = new QTableWidgetItem(strType);
                twCommand->setItem(counter-1, 1, tmp);
                
                tmp = new QTableWidgetItem(commands.at(i)->getValue());
                twCommand->setItem(counter-1, 2, tmp);
            }
		}
	}
        //QMessageBox::information(this, "commandsettings", "searchCommandList44");
	twCommand->resizeColumnToContents(1);
}

/**
* \brief to test the importprogramwizard
*
* \author Susanne Tschernegg
*/
void CommandSettings::importNewProgram()
{
        importProgramWizard->show();
        return;



    //QMessageBox::information(this, "commandsettings", "importNewProgram");
    bool checked = ui.pbImportProgram->isChecked();
  /*  if(checked)
    {
        int currRow = twCommand->currentRow();
        if(currRow>=0)
        {
            if(!allCommandValuesSet(currRow))
            {
                ui.pbImportProgram->setChecked(false);
                return;
            }
            if(commandEdited)
            {
                if(commandNameExists(twCommand->item(currRow, 0)->text(), currRow))
                {
                    ui.pbImportProgram->setChecked(false);
                    return;
                }
                if(commandValueExists(twCommand->item(currRow, 2)->text(), currRow))
                {
                    ui.pbImportProgram->setChecked(false);
                    return;
                }
            }
            deactivateAllCbs();
        }
    }*/
    
    twCommand->setCurrentCell(0,0);
    if(commandEdited)
        return;
        //QMessageBox::information(this,"commandsettings","nach da if schleife");
    ui.leSearchCommand->clear();
    
    showAllCommands();
    
    ui.pbImportPlace->setDisabled(checked);
    ui.pbNewCommand->setDisabled(checked);
    ui.pbEditCommand->setDisabled(checked);
    ui.pbDeleteCommand->setDisabled(checked);
    ui.leSchluesselWort->setDisabled(checked);
    ui.leSearchCommand->setDisabled(checked);
    ui.pbClearSearchCommand->setDisabled(checked);
    ui.cbShowCommand->setDisabled(checked);
    twCommand->setDisabled(checked);
    
//     if(checked)
//         importProgramWizard->show();
//     else
//         importProgramWizard->hide();
}

/**
* \brief slot: the signal will be emited in the ImportProgramPage
*
* \author Susanne Tschernegg
*/
void CommandSettings::insertCommand(Command* command)
{
    //QMessageBox::information(this, "commandsettings", "insertCommand");
	int rows = twCommand->rowCount();
	twCommand->insertRow(rows);
    
    QTableWidgetItem *tmp = new QTableWidgetItem(command->getName());
    tmp->setData(Qt::UserRole, command->getName());
	twCommand->setItem(rows, 0, tmp);
    
    CommandType ctype = command->getType();
    QString strType;
    if (ctype ==place)
        strType = tr("Orte");
    else if (ctype == type)
        strType = tr("Sonderzeichen");
    else strType = tr("Programme");
    twCommand->setItem(rows, 1, new	QTableWidgetItem(strType));
    
	twCommand->setItem(rows, 2, new QTableWidgetItem(command->getValue()));

    for(int i=0; i<twCommand->rowCount(); i++)  ///-----------------------------
    {
        if((i!=rows) && (QString::compare(command->getName(), twCommand->item(i, 0)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::information(this, tr("Name existiert bereits"), tr("Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte ändern Sie den Namen des neuen Kommandos.\n\nWollen Sie den Wizard nun verlassen? (Klicken Sie Nein, um den Namen zu ändern!"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes)
             {
                //commandLoader->deleteCommand(ui.twCommand->item(rows, 0)->text(), ui.twCommand->item(rows, 2)->text());
                 if(!twCommand->item(rows, 0)->data(Qt::UserRole).isNull())
                    commandLoader->deleteCommand(twCommand->item(rows, 0)->data(Qt::UserRole).toString());
                 //else commandLoader->deleteCommand("");
                twCommand->removeRow(rows);
                emit changeExistingName(false);
             }
             else
             {
                if(!twCommand->item(rows, 0)->data(Qt::UserRole).isNull())
                    commandLoader->deleteCommand(twCommand->item(rows, 0)->data(Qt::UserRole).toString());       //ui.twCommand->item(rows, 0)->text(), ui.twCommand->item(rows, 2)->text());
                //else commandLoader->deleteCommand("");
                twCommand->removeRow(rows);
                emit changeExistingName(true);
             }
        }
    }
}


/**
* \brief if a commandname already exists, a messagebox will be opend and you can either change the name or the command will be deleted.
*
* \author Susanne Tschernegg
*/
bool CommandSettings::commandNameExists(QString name, int prevRow)
{
    //QMessageBox::information(this, "commandsettings", "commandNameExists");
    for(int i=0; i<twCommand->rowCount(); i++)  //-----------------
    {
        if((i!=prevRow) && (!twCommand->item(i, 0)->data(Qt::UserRole).isNull()) && (QString::compare(name, twCommand->item(i, 0)->data(Qt::UserRole).toString(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::information(this, tr("Name existiert bereits"), tr("Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte geben Sie einen anderen Namen für dieses Kommando ein.\n\nWollen Sie das Kommando löschen? (Klickes Sie Nein, um den Namen jetzt zu ändern)"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes)
             {
                 if(!twCommand->item(prevRow, 0)->data(Qt::UserRole).isNull())
                 {
                    commandLoader->deleteCommand(twCommand->item(prevRow, 0)->data(Qt::UserRole).toString());//, ui.twCommand->item(prevRow, 2)->text());
                 }
              /*   else
                 {
                     commandLoader->deleteCommand(ui.twCommand->item(prevRow, 0)->text(), ui.twCommand->item(prevRow, 2)->text());
                 }*/
                //ui.twCommand->editItem(ui.twCommand->item(0,0)
                commandEdited=false;
                twCommand->removeRow(prevRow);
            }
            else
            {
                twCommand->selectColumn(0);
                twCommand->selectRow(prevRow);
                twCommand->setCurrentCell(prevRow,0);   //10:31
                twCommand->editItem(twCommand->item(prevRow, 0));
                commandEdited=true;
                return true;    //10:14
            }
            //return true;      //10:14
        }
    }
    return false;
}

/**
* \brief if the value of an command already exists, the users attention will be called and the user can change the value or the command will be deleted.
*
* \author Susanne Tschernegg
*/
bool CommandSettings::commandValueExists(QString value, int prevRow)
{
    //QMessageBox::information(this, "commandsettings", "commandValueExists");
    for(int i=0; i<twCommand->rowCount(); i++)          //-----------------------
    {
        if((i!=prevRow) && (QString::compare(value, twCommand->item(i, 2)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::critical(this, tr("Wert existiert bereits"), tr("Dieser Wert existiert bereits!\n\nWollen Sie den Wert jetzt ändern? (Klickes Sie Nein, um den Wert jetzt zu behalten)"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes) 
             {
                twCommand->selectColumn(2);
                twCommand->selectRow(prevRow);
                twCommand->editItem(twCommand->item(prevRow, 2));
                twCommand->setCurrentCell(prevRow,0);  //10:35
                commandEdited=true;
                return true;
             }
            //return true;
        }
    }
    // commandEdited=false;
    return false;
}

/**
* \brief checks, whether all values of a comment were set, or if the value isn't reasonable.
*
* \author Susanne Tschernegg
*/
bool CommandSettings::allCommandValuesSet(int prevRow)
{
    // QMessageBox::information(this, "commandsettings", "allCommandValuesSet");
    if(prevRow>=twCommand->rowCount() || prevRow<0)
        return true;
    if((twCommand->item(prevRow, 0)->text()==NULL)||(twCommand->item(prevRow, 0)->text()==NULL)||
		(twCommand->item(prevRow,0)->text().trimmed()=="")||(twCommand->item(prevRow,2)->text().trimmed()==""))
	{
		int result = QMessageBox::question(this, tr("Leeres Kommandofeld"), tr("Dieses Kommando wurde nicht vollständig ausgefüllt.\nJedes Kommando muss einen Namen, Wert und Typ besitzen.\n\nWollen Sie diesen Eintrag jetzt löschen? (Klicken Sie Nein, um das Kommando jetzt zu vervollständigen)"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		 if (result == QMessageBox::Yes) 
		 {
             if((!twCommand->item(prevRow, 0)->data(Qt::UserRole).isNull()) && ((twCommand->item(prevRow, 0)->data(Qt::UserRole).toString().trimmed()!="")))
             {
               //deletes the command by the data
                commandLoader->deleteCommand(twCommand->item(prevRow, 0)->data(Qt::UserRole).toString());//, ui.twCommand->item(prevRow, 2)->text());
             }
           /*  else 
             {
               //deletes the command by the text
                 commandLoader->deleteCommand(ui.twCommand->item(prevRow, 0)->text(), ui.twCommand->item(prevRow, 2)->text());
             }*/
            // if(ui.twCommand->currentRow()==prevRow)
              //   ui.twCommand->selectRow(-1); //doesn't work
             commandEdited=false;
             twCommand->removeRow(prevRow);
		 } else
		 {
            twCommand->selectColumn(0);
			twCommand->selectRow(prevRow);
			twCommand->editItem(twCommand->item(prevRow, 0));
            twCommand->setCurrentCell(prevRow,0);  //10:35
            commandEdited=true;
		 }
		 return false;
	 }
     return true;
}

/**
* \brief deletes the combobox and writes instead the type as plain text into the cell.
*
* \author Susanne Tschernegg
*/
void CommandSettings::deactivateCB(int prevRow)
{
    //QMessageBox::information(this, "commandsettings", "deactivateCB");
    QWidget *tmpWidget = new QWidget();
	tmpWidget = twCommand->cellWidget(prevRow, 1);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		twCommand->removeCellWidget(prevRow, 1);
		twCommand->setItem(prevRow, 1, tmp);
		
		int typeInt;
		if(type == "Orte")
			typeInt = 1;
		else if (type == "Sonderzeichen")
			typeInt = 2;
		else typeInt = 0;
    
       //creates esp. replaces a command
        Command *newCommand = new Command(twCommand->item(prevRow,0)->text(), CommandType(typeInt), twCommand->item(prevRow,2)->text());
        if((!twCommand->item(prevRow,0)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(prevRow,0)->data(Qt::UserRole).toString())))
        {
           //replaces the command
            commandLoader->replaceCommand(twCommand->item(prevRow,0)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(prevRow,0)->setData(Qt::UserRole, twCommand->item(prevRow,0)->text());
        }
        else
        {
            commandLoader->addCommand(newCommand);
            twCommand->item(prevRow,0)->setData(Qt::UserRole, twCommand->item(prevRow,0)->text());
        }
	}
}

/**
* \brief sets all widgets disabled = false
*
* \author Susanne Tschernegg
*/
void CommandSettings::setWidgetsDisabled()
{
    //QMessageBox::information(this, "commandsettings", "setWidgetsDisabled");
    ui.pbImportProgram->setChecked(false);
    
    ui.pbImportPlace->setDisabled(false);
    ui.pbNewCommand->setDisabled(false);
    ui.pbEditCommand->setDisabled(false);
    ui.pbDeleteCommand->setDisabled(false);
    ui.leSchluesselWort->setDisabled(false);
    ui.leSearchCommand->setDisabled(false);
    ui.pbClearSearchCommand->setDisabled(false);
    ui.cbShowCommand->setDisabled(false);
    twCommand->setDisabled(false);
}

/**
* \brief shows all existing commands
*
* \author Susanne Tschernegg
*/
void CommandSettings::showAllCommands()
{
   // QMessageBox::information(this, "commandsettings", "showAllCommands");
/*	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
	QTableWidgetItem *tmp;
	CommandList commands = commandLoader->getCommands();
	for (int i=0; i < commands.count(); i++)
	{
		CommandType ctype = commands.at(i)->getType();
		QString strType;
			
		if (ctype ==place)
			strType = tr("Orte");
		else if (ctype == type)
			strType = tr("Sonderzeichen");
		else strType = tr("Programme");
        
        ui.twCommand->setRowCount(commands.count());
        
        tmp = new QTableWidgetItem(commands.at(i)->getName());
        ui.twCommand->setItem(i, 0, tmp);
        
        tmp = new QTableWidgetItem(strType);
        ui.twCommand->setItem(i, 1, tmp);
        
        tmp = new QTableWidgetItem(commands.at(i)->getValue());
        ui.twCommand->setItem(i, 2, tmp);
	}
	ui.twCommand->resizeColumnToContents(1);*/
    ui.cbShowCommand->setCurrentIndex(0);
}

void CommandSettings::checkValuesAfterReturnPressed()
{
    //QMessageBox::information(this,"commandsettings","checkValuesAfterEnterPressed");
    int currRow = twCommand->currentRow();
    int currCol = twCommand->currentColumn();
    //deactivateCB(currRow);
    if(currRow!=0)      //11:30
        twCommand->setCurrentCell(0,0);
    else if(twCommand->rowCount()>1)    //11:30
        twCommand->setCurrentCell(1,0);
    if(currRow<twCommand->rowCount())
        twCommand->setCurrentCell(currRow,currCol);
    deactivateCB(currRow);    //11:30, 13:44
    commandEdited = false;  //13:45
}

void CommandSettings::importNewPlace()
{
   // QMessageBox::information(this, "commandsettings", "importNewPlace");
    bool checked = ui.pbImportPlace->isChecked();
      
    twCommand->setCurrentCell(0,0);
    if(commandEdited)
        return;
    ui.leSearchCommand->clear();
    
    showAllCommands();
    
    ui.pbImportProgram->setDisabled(checked);
    ui.pbNewCommand->setDisabled(checked);
    ui.pbEditCommand->setDisabled(checked);
    ui.pbDeleteCommand->setDisabled(checked);
    ui.leSchluesselWort->setDisabled(checked);
    ui.leSearchCommand->setDisabled(checked);
    ui.pbClearSearchCommand->setDisabled(checked);
    ui.cbShowCommand->setDisabled(checked);
    twCommand->setDisabled(checked);
    
    if(checked)
        importPlaceWizard->show();
    else
        importPlaceWizard->hide();
}

