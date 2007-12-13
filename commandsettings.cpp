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
#include <QVariant>
#include "command.h"
#ifdef __WIN32
#include <windows.h>
#include "qwindowsstyle.h"
#endif
#include "importprogramwizard.h"
#include "importplacewizard.h"
#include "logger.h"
#include "settings.h"
#include <QLibrary>
#include "icondialog.h"
#include "iconbutton.h"
#include "windowsresourcehandler.h"
#include <QRegExp>

/**
*   \brief constructor
*   @author Susanne Tschernegg
*/
CommandSettings::CommandSettings(QWidget* parent): SystemWidget(tr("Kommandos"), QIcon(":/images/icons/emblem-system.svg"), tr("Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten"), parent)
{
    ui.setupUi(this);
    //iconButton = new IconButton();
    commandEdited = false;
    twCommand = new ReturnTableWidget(this);
    twCommand->setColumnCount(5);
    QStringList headerLabels;
    headerLabels.append(tr("Icon"));
    headerLabels.append(tr("Name"));
    headerLabels.append(tr("Typ"));
    headerLabels.append(tr("Wert"));
    headerLabels.append(tr("Arbeitspfad"));
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
    commandLoader = new XMLCommand("conf/commands.xml");
    connect(importProgramWizard, SIGNAL(commandCreated(Command*)), this, SLOT(insertCommand(Command*)));   
    connect(importProgramWizard, SIGNAL(finished(int)), this, SLOT(setWidgetsDisabled()));
    connect(this, SIGNAL(changeExistingName(bool)), importProgramWizard, SLOT(changeName(bool)));
    connect(twCommand, SIGNAL(returnPressed()), this, SLOT(checkValuesAfterReturnPressed()));
    connect(ui.pbClearSearchCommand, SIGNAL(clicked()),this,SLOT(checkValuesAfterReturnPressed()));
    connect(ui.pbImportPlace, SIGNAL(clicked()), this, SLOT(importNewPlace()));
    connect(importPlaceWizard, SIGNAL(commandCreated(Command*)), this, SLOT(insertCommand(Command*)));   
    connect(importPlaceWizard, SIGNAL(finished(int)), this, SLOT(setWidgetsDisabled()));
    connect(this, SIGNAL(changeExistingName(bool)), importPlaceWizard, SLOT(changeName(bool)));
   // connect(iconButton, SIGNAL(iconSelected(QString)), this, SLOT(changeIcon(QString)));
   twCommand->resizeColumnToContents(2);
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
CommandSettings::~CommandSettings()
{
    
}

/**
*   \brief saves all commands. New created commands will be appended to the commandlist and existing commands will be updated.
*   @author Susanne Tschernegg
*/
bool CommandSettings::apply()
{
	Logger::log(tr("[INF] Speichere Kommandos..."));
    
    int currRow = 0;
    QString type;
    Command *newCommand;
    for(int i=0; i<twCommand->rowCount(); i++)
    {
        type = twCommand->item(i,2)->text();
        int typeInt = getTypeNumber(type);

        QString iconResources = "";
	    if(dynamic_cast<IconButton*>(twCommand->cellWidget(i,0)))
        {
            IconButton *iconButton = dynamic_cast<IconButton*>(twCommand->cellWidget(i,0));
            iconResources = iconButton->getIconName();
        }
        
        //creates a new command or replaces it
        //Command(QString name, CommandType type, QString value, QString iconPath = "", QString workingDirectory="")
        newCommand = new Command(twCommand->item(i,1)->text(), CommandType(typeInt), twCommand->item(i,3)->text(), iconResources, twCommand->item(i,4)->text());
        
        if((!twCommand->item(i,1)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(i,1)->data(Qt::UserRole).toString())))
        {
            //replaces the old command
            commandLoader->replaceCommand(twCommand->item(i,1)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(i,1)->setData(Qt::UserRole, twCommand->item(i,1)->text());
        }
        else
        {
            //creates a new command
            commandLoader->addCommand(newCommand);
            twCommand->item(i,1)->setData(Qt::UserRole, twCommand->item(i,1)->text());
        }
    }
    
	bool success = commandLoader->save(Settings::get("PathToCommands").toString());
    emit commandsChanged();
	Settings::set("Commands/Keyword", ui.leKeyword->text());
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
	CommandList commands = commandLoader->getCommands();
	twCommand->setRowCount(commands.count());
    QTableWidgetItem *tmp;
    ui.cbShowCommand->setCurrentIndex(0);
    ui.leSearchCommand->clear();
	Logger::log(tr("[INF] Habe ")+QString::number(commands.count())+tr("Kommandos gefunden"));
	for (int i=0; i < commands.count(); i++)
	{
        QString resourceId = commands.at(i)->getIconPath();
        
        QIcon icon = getIconFromResource(resourceId);
        
        IconButton *iconButton = new IconButton(this);
        iconButton->setIcon(icon);
        iconButton->setIconName(resourceId);
        
        twCommand->setCellWidget(i, 0, iconButton);
        connect(iconButton, SIGNAL(clicked()), iconButton, SLOT(changeIcon()));
        
		tmp = new QTableWidgetItem(commands.at(i)->getName());
        tmp->setData(Qt::UserRole, commands.at(i)->getName());
		twCommand->setItem(i, 1, tmp);
		
		CommandType ctype = commands.at(i)->getType();
		QString strType = getTypeName(ctype);
		
		twCommand->setItem(i, 2, new QTableWidgetItem(strType));
		twCommand->resizeColumnToContents(2);
		
		twCommand->setItem(i, 3, new QTableWidgetItem(commands.at(i)->getValue()));
        
        twCommand->setItem(i, 4, new QTableWidgetItem(commands.at(i)->getWorkingDirectory()));
	}
    commandsCount = commands.count();

	ui.leKeyword->setText(Settings::get("Commands/Keyword").toString());
    commandEdited = false;
    return true;
}

/**
*   \brief if any commandline has still a combobox to choose a type for the command, the combobox will be deleted and
        writes the type as plain text instead in the cell.
*   @author Susanne Tschernegg
*/
void CommandSettings::deactivateAllCbs()
{
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
        
        int typeInt = getTypeNumber(type);

        int currRow = twCommand->currentRow();

        //creates a new command resp. replaces it
        Command *newCommand = new Command(twCommand->item(currRow,1)->text(), CommandType(typeInt), twCommand->item(currRow,3)->text());
        if((!twCommand->item(currRow,1)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(currRow,1)->data(Qt::UserRole).toString())))
        {
            //replaces the old command
            commandLoader->replaceCommand(twCommand->item(currRow,1)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(currRow,1)->setData(Qt::UserRole, twCommand->item(currRow,1)->text());
        }
        else
        {
            commandLoader->addCommand(newCommand);
            twCommand->item(currRow,1)->setData(Qt::UserRole, twCommand->item(currRow,1)->text());
        }
    }
    commandEdited = false;
}


/**
* \brief The TreeWidgetItem will be modified, to change the values. The type of a command can be chosen from a ComboBox.
*
*\author SusanneTschernegg
*/
void CommandSettings::editCommand(int row, int column)
{
    if(twCommand->currentRow()<0)
        return;
    commandEdited = true;
    if(row<0)
	    row = twCommand->currentRow();
    
    QWidget *tmpWidget = new QWidget();
	tmpWidget = twCommand->cellWidget(row, 2);
	if(tmpWidget==NULL)
	{
	    QString typeStr = twCommand->item(row,2)->text();
	    QComboBox *cbType = new QComboBox();
	    cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	    cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	    cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
        int posCb = getTypeNumber(typeStr);

        cbType->setCurrentIndex(posCb);
        twCommand->setCellWidget(row, 2, cbType);
    }
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
    if(commandEdited)
    {
        if(twCommand->currentRow()!=0)
            twCommand->setCurrentCell(0,0);
        else if(twCommand->rowCount()>1)
            twCommand->setCurrentCell(1,0);
    }
    
    if(commandEdited)
        return;
    ui.leSearchCommand->clear();
    showAllCommands();
	int rows = twCommand->rowCount();
    twCommand->setRowCount(rows);
	twCommand->insertRow(rows);
    
#ifdef __WIN32
    WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
    QIcon icon = windowsResourceHandler->retrieveIcon("shell32.dll", 12);
    
    IconButton *iconButton = new IconButton(this);
    iconButton->setIcon(icon);
    iconButton->setIconName("shell32.dll,12");
    
    twCommand->setCellWidget(rows, 0, iconButton);
    connect(iconButton, SIGNAL(clicked()), iconButton, SLOT(changeIcon()));
#endif
    
	QTableWidgetItem *tmp = new QTableWidgetItem();
	twCommand->setItem(rows, 1, tmp);
    
	QComboBox *cbType = new QComboBox(twCommand);
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
    
	twCommand->setCellWidget(rows, 2, cbType);
	twCommand->setItem(rows, 3, new QTableWidgetItem());
    twCommand->setItem(rows, 4, new QTableWidgetItem());
    twCommand->setCurrentCell(rows,1);
    twCommand->editItem(twCommand->item(rows, 1));
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
    int currRow = twCommand->currentRow();
    
    if(currRow!=0)
        twCommand->setCurrentCell(0,0);
    else if(twCommand->rowCount()>1)
        twCommand->setCurrentCell(1,0);
    
    if(commandEdited)
    {
        return;
    }
    
    if(rowCount==twCommand->rowCount())
    {
        if(!twCommand->item(currRow,1)->data(Qt::UserRole).isNull())
            commandLoader->deleteCommand(twCommand->item(currRow, 1)->data(Qt::UserRole).toString());//, ui.twCommand->item(currRow, 2)->text());
        twCommand->removeRow(currRow);
    }
    commandEdited = false;
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
	twCommand->setCellWidget(twCommand->currentRow(), 2, cbType);
}

/**
* \brief writes the type - which was chosen bevor in the combobox - in plaintext out
*
* \author Susanne Tschernegg
*/
void CommandSettings::checkAndAddCommandValues(int currRow, int currCol, int prevRow, int prevCol)
{
    if(prevRow < 0)
        return;

    if((prevCol==1) && (commandEdited))// && (ui.twCommand->item(prevRow, 0)->text()!=NULL) && ((ui.twCommand->item(prevRow,0)->text().trimmed()!="")))
    {
        if(commandNameExists(twCommand->item(prevRow, 1)->text(),prevRow))
        {
            //if the name exists
            ui.pbImportProgram->setChecked(false);
            commandEdited = true;
             return;
        }
    }
    
    //if the column value was selected
    else if((prevCol==3) && (commandEdited))// && (ui.twCommand->item(prevRow, 2)->text()!=NULL) && ((ui.twCommand->item(prevRow,2)->text().trimmed()!="")))
    {
        if(commandValueExists(twCommand->item(prevRow, 3)->text(), prevRow))
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
		return;
    }
    
    //if the commandvalues weren't set at all
    if(!allCommandValuesSet(prevRow) && commandEdited)
    {
        ui.pbImportProgram->setChecked(false);
        commandEdited = true;
        return;
    }
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
            if(commandNameExists(twCommand->item(currRow, 1)->text(), currRow))
            {
                ui.pbImportProgram->setChecked(false);
                return false;
            }
            if(commandValueExists(twCommand->item(currRow, 3)->text(), currRow))
            {
                ui.pbImportProgram->setChecked(false);
                return false;
            }
        }
        //deactivateAllCbs();
    }
	twCommand->clearContents();
	twCommand->setRowCount(0);
    commandEdited = false;
    return init();
}

/**
* \brief shows only the commands, which have the selected type of the combobox cbShowCommand
*
* \author Susanne Tschernegg
*/
void CommandSettings::showOnlyCommands()
{   
    if(commandEdited)
    {
        if(ui.cbShowCommand->currentIndex()!=0)
        {
            ui.cbShowCommand->setCurrentIndex(0);
            return;
        }
        
        if(!(twCommand->currentRow()<0))
        {
            int currRow = twCommand->currentRow();
            
            if(currRow!=0)
                twCommand->setCurrentCell(0,0);
            else if(twCommand->rowCount()>1)
                twCommand->setCurrentCell(1,0);
        }
        return;
    }

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
            if(commandNameExists(twCommand->item(currRow, 1)->text(), currRow))
            {
                return;
            }
            if(commandValueExists(twCommand->item(currRow, 3)->text(), currRow))
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
		QString strType = getTypeName(ctype);
        
		if((strType==currType)||(currType=="Alles"))
		{
			counter ++;
			twCommand->setRowCount(counter);
            
            QString resourceId = commands.at(i)->getIconPath();
            
            QIcon icon = getIconFromResource(resourceId);
            
            IconButton *iconButton = new IconButton(this);
            iconButton->setIcon(icon);
            iconButton->setIconName(resourceId);
            
            twCommand->setCellWidget(counter-1, 0, iconButton);
            connect(iconButton, SIGNAL(clicked()), iconButton, SLOT(changeIcon()));   //connect
			
			tmp = new QTableWidgetItem(commands.at(i)->getName());
            tmp->setData(Qt::UserRole, commands.at(i)->getName());
			twCommand->setItem(counter-1, 1, tmp);
			
			tmp = new QTableWidgetItem(strType);
			twCommand->setItem(counter-1, 2, tmp);
			
			tmp = new QTableWidgetItem(commands.at(i)->getValue());
			twCommand->setItem(counter-1, 3, tmp);
		}
	}
	twCommand->resizeColumnToContents(2);
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
    if((ui.leSearchCommand->text()=="") && (!commandEdited))
    {
        CommandList allCommands = commandLoader->getCommands();
        twCommand->clearContents();
        twCommand->setRowCount(0);
	    QTableWidgetItem *tmp;
        int counter = 0;
        for(int z=0; z<allCommands.count(); z++)
        {
            CommandType ctype = allCommands.at(z)->getType();
			QString strType = getTypeName(ctype);            
            
            if((ui.cbShowCommand->currentIndex()==0) || (strType==ui.cbShowCommand->currentText()))
            {
                counter++;
                twCommand->setRowCount(counter);
                
                QString resourceId = allCommands.at(z)->getIconPath();
        
                QIcon icon = getIconFromResource(resourceId);
                
                IconButton *iconButton = new IconButton(this);
                iconButton->setIcon(icon);
                iconButton->setIconName(resourceId);
                
                twCommand->setCellWidget(counter-1, 0, iconButton);
                connect(iconButton, SIGNAL(clicked()), iconButton, SLOT(changeIcon()));
                
                tmp = new QTableWidgetItem(allCommands.at(z)->getName());
                tmp->setData(Qt::UserRole, allCommands.at(z)->getName());
                twCommand->setItem(counter-1, 1, tmp);
                
                CommandType ctype = allCommands.at(z)->getType();
                QString strType = getTypeName(ctype);
                
                tmp = new QTableWidgetItem(strType);
                twCommand->setItem(counter-1, 2, tmp);
                
                tmp = new QTableWidgetItem(allCommands.at(z)->getValue());
                twCommand->setItem(counter-1, 3, tmp);
            }
        }
        twCommand->resizeColumnToContents(1);
        return;
    }
    
    int prevRow = twCommand->currentRow();

    if(ui.leSearchCommand->text()!="")
        if(twCommand->rowCount()>0 && prevRow!=0)
            twCommand->setCurrentCell(1,0);

        else if(twCommand->rowCount()>1)
            twCommand->setCurrentCell(0,0);

    if(!twCommand->item(prevRow,1))
    {
        if(!prevRow>=twCommand->rowCount())
        {
            return;
        }
    }
    else
    {
        if((twCommand->item(prevRow, 1)->text()==NULL)||(twCommand->item(prevRow, 1)->text()==NULL)||
            (twCommand->item(prevRow,1)->text().trimmed()=="")||(twCommand->item(prevRow,3)->text().trimmed()==""))
        {
            commandEdited = true;
            ui.leSearchCommand->clear();
            return;
        }
    }
    
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
            QString strType = getTypeName(ctype);
            
            if((ui.cbShowCommand->currentIndex()==0) || (strType==ui.cbShowCommand->currentText()))
            {
                counter ++;
                twCommand->setRowCount(counter);
                
                QString resourceId = commands.at(i)->getIconPath();
        
                QIcon icon = getIconFromResource(resourceId);
                
                IconButton *iconButton = new IconButton(this);
                iconButton->setIcon(icon);
                iconButton->setIconName(resourceId);
                
                twCommand->setCellWidget(counter-1, 0, iconButton);
                connect(iconButton, SIGNAL(clicked()), iconButton, SLOT(changeIcon()));
                
                tmp = new QTableWidgetItem(commands.at(i)->getName());
                tmp->setData(Qt::UserRole, commands.at(i)->getName());
                twCommand->setItem(counter-1, 1, tmp);
                
                tmp = new QTableWidgetItem(strType);
                twCommand->setItem(counter-1, 2, tmp);
                
                tmp = new QTableWidgetItem(commands.at(i)->getValue());
                twCommand->setItem(counter-1, 3, tmp);
            }
		}
	}
	twCommand->resizeColumnToContents(2);
}

/**
* \brief to open the importprogramwizard
*
* \author Susanne Tschernegg
*/
void CommandSettings::importNewProgram()
{
    bool checked = ui.pbImportProgram->isChecked();
 
    twCommand->setCurrentCell(0,0);
    if(commandEdited)
        return;
    
    ui.leSearchCommand->clear();
    
    showAllCommands();
    
    ui.pbImportPlace->setDisabled(checked);
    ui.pbNewCommand->setDisabled(checked);
    ui.pbEditCommand->setDisabled(checked);
    ui.pbDeleteCommand->setDisabled(checked);
    ui.leKeyword->setDisabled(checked);
    ui.leSearchCommand->setDisabled(checked);
    ui.pbClearSearchCommand->setDisabled(checked);
    ui.cbShowCommand->setDisabled(checked);
    twCommand->setDisabled(checked);
    
     if(checked)
         importProgramWizard->show();
     else
         importProgramWizard->hide();
}

/**
* \brief slot: the signal will be emited in the ImportProgramPage
*
* \author Susanne Tschernegg
*/
void CommandSettings::insertCommand(Command* command)
{
	int rows = twCommand->rowCount();
	twCommand->insertRow(rows);
    
#ifdef __WIN32
    WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
    QIcon icon = windowsResourceHandler->retrieveIcon("shell32.dll", 12);
    
    IconButton *iconButton = new IconButton(this);
    iconButton->setIcon(icon);
    iconButton->setIconName("shell32.dll,12");
    
    twCommand->setCellWidget(rows, 0, iconButton);
    connect(iconButton, SIGNAL(clicked()), iconButton, SLOT(changeIcon()));
#endif
    
    QTableWidgetItem *tmp = new QTableWidgetItem(command->getName());
    tmp->setData(Qt::UserRole, command->getName());
	twCommand->setItem(rows, 1, tmp);
    
    CommandType ctype = command->getType();
    QString strType = getTypeName(ctype);
    
    twCommand->setItem(rows, 2, new	QTableWidgetItem(strType));
    
    twCommand->setItem(rows, 3, new QTableWidgetItem(command->getValue()));
    
    twCommand->setItem(rows, 4, new QTableWidgetItem(""));

    for(int i=0; i<twCommand->rowCount(); i++)
    {
        if((i!=rows) && (QString::compare(command->getName(), twCommand->item(i, 1)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::information(this, tr("Name existiert bereits"), tr("Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte ändern Sie den Namen des neuen Kommandos.\n\nWollen Sie den Wizard nun verlassen? (Klicken Sie Nein, um den Namen zu ändern!"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes)
             {
                 if(!twCommand->item(rows, 1)->data(Qt::UserRole).isNull())
                    commandLoader->deleteCommand(twCommand->item(rows, 1)->data(Qt::UserRole).toString());
                twCommand->removeRow(rows);
                emit changeExistingName(false);
             }
             else
             {
                if(!twCommand->item(rows, 1)->data(Qt::UserRole).isNull())
                    commandLoader->deleteCommand(twCommand->item(rows, 1)->data(Qt::UserRole).toString());
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
    for(int i=0; i<twCommand->rowCount(); i++)
    {
        if((i!=prevRow) && (!twCommand->item(i, 1)->data(Qt::UserRole).isNull()) && (QString::compare(name, twCommand->item(i, 1)->data(Qt::UserRole).toString(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::information(this, tr("Name existiert bereits"), tr("Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte geben Sie einen anderen Namen für dieses Kommando ein.\n\nWollen Sie das Kommando löschen? (Klicken Sie Nein, um den Namen jetzt zu ändern)"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes)
             {
                 if(!twCommand->item(prevRow, 0)->data(Qt::UserRole).isNull())
                 {
                    commandLoader->deleteCommand(twCommand->item(prevRow, 1)->data(Qt::UserRole).toString());
                 }
                commandEdited=false;
                twCommand->removeRow(prevRow);
            }
            else
            {
                twCommand->selectColumn(1);
                twCommand->selectRow(prevRow);
                twCommand->setCurrentCell(prevRow,1);
                twCommand->editItem(twCommand->item(prevRow,1));
                commandEdited=true;
                ui.pbImportProgram->setChecked(false);
                ui.pbImportPlace->setChecked(false);
                return true;
            }
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
    for(int i=0; i<twCommand->rowCount(); i++)
    {
        if((i!=prevRow) && (QString::compare(value, twCommand->item(i, 3)->text(), Qt::CaseInsensitive)==0))
        {
            int result = QMessageBox::critical(this, tr("Wert existiert bereits"), tr("Dieser Wert existiert bereits!\n\nWollen Sie den Wert jetzt ändern? (Klickes Sie Nein, um den Wert jetzt zu behalten)"),
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            if (result == QMessageBox::Yes) 
             {
                twCommand->selectColumn(3);
                twCommand->selectRow(prevRow);
                twCommand->editItem(twCommand->item(prevRow, 3));
                twCommand->setCurrentCell(prevRow,1);
                commandEdited=true;
                ui.pbImportProgram->setChecked(false);
                ui.pbImportPlace->setChecked(false);
                return true;
             }
        }
    }
    return false;
}

/**
* \brief checks, whether all values of a comment were set, or if the value isn't reasonable.
*
* \author Susanne Tschernegg
*/
bool CommandSettings::allCommandValuesSet(int prevRow)
{
    if(prevRow>=twCommand->rowCount() || prevRow<0)
        return true;
    if((twCommand->item(prevRow, 1)->text()==NULL)||(twCommand->item(prevRow, 1)->text()==NULL)||
		(twCommand->item(prevRow,1)->text().trimmed()=="")||(twCommand->item(prevRow,3)->text().trimmed()==""))
	{
		int result = QMessageBox::question(this, tr("Leeres Kommandofeld"), tr("Dieses Kommando wurde nicht vollständig ausgef?lt.\nJedes Kommando muss einen Namen, Wert und Typ besitzen.\n\nWollen Sie diesen Eintrag jetzt löschen? (Klicken Sie Nein, um das Kommando jetzt zu vervollständigen)"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		 if (result == QMessageBox::Yes) 
		 {
             if((!twCommand->item(prevRow, 1)->data(Qt::UserRole).isNull()) && ((twCommand->item(prevRow, 0)->data(Qt::UserRole).toString().trimmed()!="")))
             {
               //deletes the command by the data
                commandLoader->deleteCommand(twCommand->item(prevRow, 1)->data(Qt::UserRole).toString());
             }
             commandEdited=false;
             twCommand->removeRow(prevRow);
		 }
         else
		 {
            twCommand->selectColumn(1);
			twCommand->selectRow(prevRow);
			twCommand->editItem(twCommand->item(prevRow, 1));
            twCommand->setCurrentCell(prevRow,1);
            commandEdited=true;
            ui.pbImportProgram->setChecked(false);
            ui.pbImportPlace->setChecked(false);
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
    QWidget *tmpWidget = new QWidget();
	tmpWidget = twCommand->cellWidget(prevRow, 2);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		twCommand->removeCellWidget(prevRow, 2);
		twCommand->setItem(prevRow, 2, tmp);
		
		int typeInt = getTypeNumber(type);
    
       //creates esp. replaces a command
        Command *newCommand = new Command(twCommand->item(prevRow,1)->text(), CommandType(typeInt), twCommand->item(prevRow,3)->text());
        if((!twCommand->item(prevRow,1)->data(Qt::UserRole).isNull()) && (commandLoader->commandExists(twCommand->item(prevRow,1)->data(Qt::UserRole).toString())))
        {
           //replaces the command
            commandLoader->replaceCommand(twCommand->item(prevRow,1)->data(Qt::UserRole).toString(), newCommand);
            twCommand->item(prevRow,1)->setData(Qt::UserRole, twCommand->item(prevRow,1)->text());
        }
        else
        {
            commandLoader->addCommand(newCommand);
            twCommand->item(prevRow,1)->setData(Qt::UserRole, twCommand->item(prevRow,1)->text());
        }
	}
    commandEdited = false;
}

/**
* \brief sets all widgets disabled = false
*
* \author Susanne Tschernegg
*/
void CommandSettings::setWidgetsDisabled()
{
    ui.pbImportProgram->setChecked(false);
    ui.pbImportPlace->setChecked(false);
    
    ui.pbImportProgram->setDisabled(false);
    ui.pbImportPlace->setDisabled(false);
    ui.pbNewCommand->setDisabled(false);
    ui.pbEditCommand->setDisabled(false);
    ui.pbDeleteCommand->setDisabled(false);
    ui.leKeyword->setDisabled(false);
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
    ui.cbShowCommand->setCurrentIndex(0);
}

/**
* \brief s
*
* \author Susanne Tschernegg
*/
void CommandSettings::checkValuesAfterReturnPressed()
{
    int currRow = twCommand->currentRow();
    int currCol = twCommand->currentColumn();
    if(currRow!=0)
        twCommand->setCurrentCell(0,0);
    else if(twCommand->rowCount()>1)
        twCommand->setCurrentCell(1,0);
    if(currRow<twCommand->rowCount())
        twCommand->setCurrentCell(currRow,currCol);
    deactivateCB(currRow);
    commandEdited = false;
}

/**
* \brief
*
* \author Susanne Tschernegg
*/
void CommandSettings::importNewPlace()
{
    bool checked = ui.pbImportPlace->isChecked();
    twCommand->setCurrentCell(0,0);
    if(commandEdited)
    {
        return;
    }
    ui.leSearchCommand->clear();
    
    showAllCommands();
    
    ui.pbImportProgram->setDisabled(checked);
    ui.pbNewCommand->setDisabled(checked);
    ui.pbEditCommand->setDisabled(checked);
    ui.pbDeleteCommand->setDisabled(checked);
    ui.leKeyword->setDisabled(checked);
    ui.leSearchCommand->setDisabled(checked);
    ui.pbClearSearchCommand->setDisabled(checked);
    ui.cbShowCommand->setDisabled(checked);
    twCommand->setDisabled(checked);
    
    if(checked)
        importPlaceWizard->show();
    else
        importPlaceWizard->hide();
}

/**
* \brief
*
* \author Susanne Tschernegg
*/
void CommandSettings::changeIcon(QString resourceId)
{
    QMessageBox::information(this,"commandsettigs::changeIcon","icon arrived");
}

/**
* \brief
*
* \author Susanne Tschernegg
*/
QIcon CommandSettings::getIconFromResource(QString resourceId)
{
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
    return icon;
}

/**
* \brief
*
* \author Susanne Tschernegg
*/
QString CommandSettings::getTypeName(CommandType ctype)
{
    QString strType = "";
    if (ctype == place)
        strType = tr("Orte");
    else if (ctype == type)
        strType = tr("Sonderzeichen");
    else strType = tr("Programme");
    return strType;
}

/**
* \brief
*
* \author Susanne Tschernegg
*/
int CommandSettings::getTypeNumber(QString commandName)
{
    int typeInt;
    if(commandName == "Orte")
        typeInt = 1;
    else if (commandName == "Sonderzeichen")
        typeInt = 2;
    else typeInt = 0;
    return typeInt;
}
