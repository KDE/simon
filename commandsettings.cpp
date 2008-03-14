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
#include "runcommand.h"
#include "settings.h"
#include <QLibrary>
#include "icondialog.h"
#include "iconbutton.h"
#include "windowsresourcehandler.h"
#include <QRegExp>

/**
*   \brief constructor
*       Initializes the * boolean variable "commandEdited" false. It would be true, if a user changes a command.
*                       * names of the columns
*                       * an ImportPlaceWizard
*                       * an ImportProgramWizard
*       and sets the connects for the widgets.
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
CommandSettings::CommandSettings ( QWidget* parent ) : SystemWidget ( tr ( "Kommandos" ), QIcon ( ":/images/icons/system-run.svg" ), tr ( "Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten" ), parent )
{
	ui.setupUi ( this );

	guessChildTriggers ( this );

	commandEdited = false;

	ui.twCommand->setColumnCount ( 5 );
	QStringList headerLabels;
	headerLabels.append ( tr ( "Icon" ) );
	headerLabels.append ( tr ( "Name" ) );
	headerLabels.append ( tr ( "Typ" ) );
	headerLabels.append ( tr ( "Wert" ) );
	headerLabels.append ( tr ( "Arbeitspfad" ) );
	ui.twCommand->setHorizontalHeaderLabels ( headerLabels );

	ui.vboxLayout->insertWidget ( 1, ui.twCommand );

	ui.pbImportProgram->setCheckable ( true );
	ui.pbImportPlace->setCheckable ( true );
	importProgramWizard = new ImportProgramWizard ( this );
	importPlaceWizard = new ImportPlaceWizard ( this );
	commandBackend = RunCommand::getInstance();

	help = tr ( "Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten" );

	//connects
	connect ( ui.pbNewCommand, SIGNAL ( clicked() ), this, SLOT ( newCommand() ) );
	connect ( ui.pbSpecialCharacter, SIGNAL ( clicked() ), this, SLOT ( newCommand() ) );
	connect ( ui.twCommand, SIGNAL ( currentCellChanged ( int,int,int,int ) ), this, SLOT ( checkAndAddCommandValues ( int,int,int,int ) ) );
	connect ( ui.pbDeleteCommand, SIGNAL ( clicked() ), this, SLOT ( deleteCommand() ) );
	connect ( ui.twCommand, SIGNAL ( cellDoubleClicked ( int, int ) ), this, SLOT ( editCommand ( int, int ) ) );
	connect ( ui.pbEditCommand, SIGNAL ( clicked() ), this, SLOT ( editCommand() ) );
	connect ( ui.cbShowCommand, SIGNAL ( currentIndexChanged ( const QString & ) ), this, SLOT ( showOnlyCommands() ) );
	connect ( ui.pbClearSearchCommand, SIGNAL ( clicked() ), this, SLOT ( clearSearchLineEdit() ) );
	connect ( ui.leSearchCommand, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( searchCommandList() ) );
	connect ( ui.pbImportProgram, SIGNAL ( clicked() ), this, SLOT ( importNewProgram() ) );

	connect ( importProgramWizard, SIGNAL ( commandCreated ( Command* ) ), this, SLOT ( insertCommand ( Command* ) ) );
	connect ( importProgramWizard, SIGNAL ( finished ( int ) ), this, SLOT ( setWidgetsDisabled() ) );
	connect ( this, SIGNAL ( changeExistingName ( bool ) ), importProgramWizard, SLOT ( changeName ( bool ) ) );
	connect ( ui.twCommand, SIGNAL ( returnPressed() ), this, SLOT ( checkValuesAfterReturnPressed() ) );
	connect ( ui.pbClearSearchCommand, SIGNAL ( clicked() ),this,SLOT ( checkValuesAfterReturnPressed() ) );
	connect ( ui.pbImportPlace, SIGNAL ( clicked() ), this, SLOT ( importNewPlace() ) );
	connect ( importPlaceWizard, SIGNAL ( commandCreated ( Command* ) ), this, SLOT ( insertCommand ( Command* ) ) );
	connect ( importPlaceWizard, SIGNAL ( finished ( int ) ), this, SLOT ( setWidgetsDisabled() ) );
	connect ( this, SIGNAL ( changeExistingName ( bool ) ), importPlaceWizard, SLOT ( changeName ( bool ) ) );
	ui.twCommand->resizeColumnToContents ( 2 );
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
CommandSettings::~CommandSettings()
{}

/**
*   \brief saves all commands. New created commands will be appended to the commandlist and existing commands will be updated.
*   @author Susanne Tschernegg
*   @return bool
*       returns if this method succeed
*/
bool CommandSettings::apply()
{
	Logger::log ( tr ( "[INF] Speichere Kommandos..." ) );

	QString type;
	Command *newCommand;
	for ( int i=0; i<ui.twCommand->rowCount(); i++ )
	{
		type = ui.twCommand->item ( i,2 )->text();
		int typeInt = getTypeNumber ( type );

		QString iconResources = "";
		if ( dynamic_cast<IconButton*> ( ui.twCommand->cellWidget ( i,0 ) ) )
		{
			IconButton *iconButton = dynamic_cast<IconButton*> ( ui.twCommand->cellWidget ( i,0 ) );
			iconResources = iconButton->getIconName();
		}

		//creates a new command or replaces it
		//Command(QString name, CommandType type, QString value, QString iconPath = "", QString workingDirectory="")
		newCommand = new Command ( ui.twCommand->item ( i,1 )->text(), CommandType ( typeInt ), ui.twCommand->item ( i,3 )->text(), iconResources, ui.twCommand->item ( i,4 )->text() );

		if ( ( !ui.twCommand->item ( i,1 )->data ( Qt::UserRole ).isNull() ) && ( commandBackend->commandExists ( ui.twCommand->item ( i,1 )->data ( Qt::UserRole ).toString() ) ) )
		{
			//replaces the old command
			commandBackend->replaceCommand ( ui.twCommand->item ( i,1 )->data ( Qt::UserRole ).toString(), newCommand );
			ui.twCommand->item ( i,1 )->setData ( Qt::UserRole, ui.twCommand->item ( i,1 )->text() );
		}
		else
		{
			//creates a new command
			commandBackend->addCommand ( newCommand );
			ui.twCommand->item ( i,1 )->setData ( Qt::UserRole, ui.twCommand->item ( i,1 )->text() );
		}
	}
	bool success = commandBackend->save ( Settings::get ( "PathToCommands" ).toString() );
	emit commandsChanged();
	Settings::set ( "Commands/Keyword", ui.leKeyword->text() );
	return success;
}

/**
 * \brief Loads the commands from the corresponding XML file and inserts them into the list for configuring the available comamnds
 * \author Peter Grasch, Tschernegg Susanne
 *  @return bool
 *      returns wheter the method succeeds or not
 */
bool CommandSettings::init()
{
	CommandList commands = commandBackend->getCommands();
	ui.twCommand->setRowCount ( commands.count() );
	QTableWidgetItem *tmp;
	ui.cbShowCommand->setCurrentIndex ( 0 );
	ui.leSearchCommand->clear();
	Logger::log ( tr ( "[INF] Habe " ) +QString::number ( commands.count() ) +tr ( "Kommandos gefunden" ) );
	for ( int i=0; i < commands.count(); i++ )
	{
		QString resourceId = commands.at ( i )->getIconPath();

		QIcon icon = getIconFromResource ( resourceId );

		IconButton *iconButton = new IconButton ( this );
		iconButton->setIcon ( icon );
		iconButton->setIconName ( resourceId );

		ui.twCommand->setCellWidget ( i, 0, iconButton );

		tmp = new QTableWidgetItem ( commands.at ( i )->getName() );
		tmp->setData ( Qt::UserRole, commands.at ( i )->getName() );
		ui.twCommand->setItem ( i, 1, tmp );

		CommandType ctype = commands.at ( i )->getType();
		QString strType = getTypeName ( ctype );

		ui.twCommand->setItem ( i, 2, new QTableWidgetItem ( strType ) );
		ui.twCommand->resizeColumnToContents ( 2 );

		ui.twCommand->setItem ( i, 3, new QTableWidgetItem ( commands.at ( i )->getValue() ) );

		ui.twCommand->setItem ( i, 4, new QTableWidgetItem ( commands.at ( i )->getWorkingDirectory() ) );
	}
	commandsCount = commands.count();

	ui.leKeyword->setText ( Settings::get ( "Commands/Keyword" ).toString() );
	ui.twCommand->resizeColumnsToContents();
	commandEdited = false;
	return true;
}

/**
*   \brief If any commandline has a combobox to choose a type for the command, the combobox will be deleted and
*        the type will be written as plain text in the cell.
*   @author Susanne Tschernegg
*/
void CommandSettings::deactivateAllCbs()
{
	QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget ( ui.twCommand->currentRow(), 1 );
	if ( tmpWidget!=NULL )
	{
		QComboBox *cbType = ( QComboBox* ) tmpWidget;
		QString type = cbType->itemText ( cbType->currentIndex() );
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText ( type );
		ui.twCommand->removeCellWidget ( ui.twCommand->currentRow(), 1 );
		ui.twCommand->setItem ( ui.twCommand->currentRow(), 1, tmp );

		int typeInt = getTypeNumber ( type );

		int currRow = ui.twCommand->currentRow();

		//creates a new command resp. replaces it
		Command *newCommand = new Command ( ui.twCommand->item ( currRow,1 )->text(), CommandType ( typeInt ), ui.twCommand->item ( currRow,3 )->text() );
		if ( ( !ui.twCommand->item ( currRow,1 )->data ( Qt::UserRole ).isNull() ) && ( commandBackend->commandExists ( ui.twCommand->item ( currRow,1 )->data ( Qt::UserRole ).toString() ) ) )
		{
			//replaces the old command
			commandBackend->replaceCommand ( ui.twCommand->item ( currRow,1 )->data ( Qt::UserRole ).toString(), newCommand );
			ui.twCommand->item ( currRow,1 )->setData ( Qt::UserRole, ui.twCommand->item ( currRow,1 )->text() );
		}
		else
		{
			commandBackend->addCommand ( newCommand );
			ui.twCommand->item ( currRow,1 )->setData ( Qt::UserRole, ui.twCommand->item ( currRow,1 )->text() );
		}
	}
	commandEdited = false;
}


/**
* \brief The TreeWidgetItem will be modified, to change the values. The type of a command can be chosen from a ComboBox.
*
*\author SusanneTschernegg
*   @param int row
*       holds the row from the tablewidgetitem, which where selected
*   @param int column
*       holds the column from the tablewidgetitem, which where selected
*/
void CommandSettings::editCommand ( int row, int column )
{
	if ( ui.twCommand->currentRow() <0 )
		return;
	commandEdited = true;
	if ( row<0 )
		row = ui.twCommand->currentRow();

	QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget ( row, 2 );
	if ( tmpWidget==NULL )
	{
		QString typeStr = ui.twCommand->item ( row,2 )->text();
		QComboBox *cbType = new QComboBox();
		cbType->addItem ( QIcon ( ":/images/icons/system-run.svg" ),QApplication::translate ( "RunDialog", "Programme", 0, QApplication::UnicodeUTF8 ) );
		cbType->addItem ( QIcon ( ":/images/icons/folder.svg" ), QApplication::translate ( "RunDialog", "Orte", 0, QApplication::UnicodeUTF8 ) );
		cbType->addItem ( QIcon ( ":/images/icons/format-text-bold.svg" ),QApplication::translate ( "RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8 ) );
		int posCb = getTypeNumber ( typeStr );

		cbType->setCurrentIndex ( posCb );
		ui.twCommand->setCellWidget ( row, 2, cbType );
	}
	ui.twCommand->setCurrentCell ( row,column );
	ui.twCommand->editItem ( ui.twCommand->item ( row, column ) );
}

/**
* \brief a new line to set a new command will be created. The type of the command can be chosen from a combobox
*
* \author Susanne Tschernegg, Peter Grasch
*/
void CommandSettings::newCommand()
{
	if ( commandEdited )
	{
		if ( ui.twCommand->currentRow() !=0 )
			ui.twCommand->setCurrentCell ( 0,0 );
		else if ( ui.twCommand->rowCount() >1 )
			ui.twCommand->setCurrentCell ( 1,0 );
	}

	if ( commandEdited )
		return;
	ui.leSearchCommand->clear();
	showAllCommands();
	int rows = ui.twCommand->rowCount();
	ui.twCommand->setRowCount ( rows );
	ui.twCommand->insertRow ( rows );

	IconButton *iconButton = new IconButton ( this );

//setting default icon
#ifdef __WIN32
	WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
	QIcon icon = windowsResourceHandler->retrieveIcon ( "shell32.dll", 12 );
	iconButton->setIcon ( icon );
	iconButton->setIconName ( "shell32.dll,12" );
#endif

	ui.twCommand->setCellWidget ( rows, 0, iconButton );

	QTableWidgetItem *tmp = new QTableWidgetItem();
	ui.twCommand->setItem ( rows, 1, tmp );

	QComboBox *cbType = new QComboBox ( ui.twCommand );
	cbType->addItem ( QIcon ( ":/images/icons/system-run.svg" ),QApplication::translate ( "RunDialog", "Programme", 0, QApplication::UnicodeUTF8 ) );
	cbType->addItem ( QIcon ( ":/images/icons/folder.svg" ), QApplication::translate ( "RunDialog", "Orte", 0, QApplication::UnicodeUTF8 ) );
	cbType->addItem ( QIcon ( ":/images/icons/format-text-bold.svg" ),QApplication::translate ( "RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8 ) );

	ui.twCommand->setCellWidget ( rows, 2, cbType );
	ui.twCommand->setItem ( rows, 3, new QTableWidgetItem() );
	ui.twCommand->setItem ( rows, 4, new QTableWidgetItem() );
	ui.twCommand->setCurrentCell ( rows,1 );
	ui.twCommand->editItem ( ui.twCommand->item ( rows, 1 ) );
	commandEdited = true;
}

/**
* \brief a command will be deleted
*
* \author Susanne Tschernegg
*/
void CommandSettings::deleteCommand()
{
	if ( ui.twCommand->currentRow() <0 )
		return;
	int rowCount = ui.twCommand->rowCount();
	int currRow = ui.twCommand->currentRow();

	if ( currRow!=0 )
		ui.twCommand->setCurrentCell ( 0,0 );
	else if ( ui.twCommand->rowCount() >1 )
		ui.twCommand->setCurrentCell ( 1,0 );

	if ( commandEdited )
	{
		return;
	}

	if ( rowCount==ui.twCommand->rowCount() )
	{
		if ( !ui.twCommand->item ( currRow,1 )->data ( Qt::UserRole ).isNull() )
			commandBackend->deleteCommand ( ui.twCommand->item ( currRow, 1 )->data ( Qt::UserRole ).toString() );//, ui.ui.twCommand->item(currRow, 2)->text());
		ui.twCommand->removeRow ( currRow );
	}
	commandEdited = false;
}

/**
* \brief to activate the combobox with different types of a command, so that de user can choose one existing type for the command
*
* \author Susanne Tschernegg
*/
void CommandSettings::activateCb()
{
	QComboBox *cbType = new QComboBox();
	cbType->addItem ( QIcon ( ":/images/icons/system-run.svg" ),QApplication::translate ( "RunDialog", "Programme", 0, QApplication::UnicodeUTF8 ) );
	cbType->addItem ( QIcon ( ":/images/icons/folder.svg" ), QApplication::translate ( "RunDialog", "Orte", 0, QApplication::UnicodeUTF8 ) );
	cbType->addItem ( QIcon ( ":/images/icons/format-text-bold.svg" ),QApplication::translate ( "RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8 ) );
	ui.twCommand->setCellWidget ( ui.twCommand->currentRow(), 2, cbType );
}


/**
 * \brief Determines if the page is completely configured
 *
 * \author Peter Grasch
 * @return true, if we set a magic word
 */
bool CommandSettings::isComplete()
{
	return ! ( ui.leKeyword->text().isEmpty() );
}

/**
* \brief Writes the type - which was chosen bevor in the combobox - in plaintext out. Before this, the command will be checked, if there are any invalid commands in the list.
*
* \author Susanne Tschernegg
*/
void CommandSettings::checkAndAddCommandValues ( int currRow, int currCol, int prevRow, int prevCol )
{
	if ( prevRow < 0 )
		return;

	if ( ( prevCol==1 ) && ( commandEdited ) ) // && (ui.ui.twCommand->item(prevRow, 0)->text()!=NULL) && ((ui.ui.twCommand->item(prevRow,0)->text().trimmed()!="")))
	{
		if ( commandNameExists ( ui.twCommand->item ( prevRow, 1 )->text(),prevRow ) )
		{
			//if the name exists
			ui.pbImportProgram->setChecked ( false );
			commandEdited = true;
			return;
		}
	}

	//if the column value was selected
	else if ( ( prevCol==3 ) && ( commandEdited ) ) // && (ui.ui.twCommand->item(prevRow, 2)->text()!=NULL) && ((ui.ui.twCommand->item(prevRow,2)->text().trimmed()!="")))
	{
		if ( commandValueExists ( ui.twCommand->item ( prevRow, 3 )->text(), prevRow ) )
		{
			//if the command has been deleted
			ui.pbImportProgram->setChecked ( false );
			commandEdited = true;
			return;
		}
	}

	//if the row hasn't been changed
	if ( ( currRow == prevRow ) ) //&&(!prevRow<0))//&&(onlyItemChanged))
	{
		return;
	}

	//if the commandvalues weren't set at all
	if ( !allCommandValuesSet ( prevRow ) && commandEdited )
	{
		ui.pbImportProgram->setChecked ( false );
		commandEdited = true;
		return;
	}
	deactivateCB ( prevRow );
	commandEdited = false;
}

/**
* \brief reloads the commands
*
* \author Susanne Tschernegg
*   @return bool
*       returns true, if the method init() succeed otherwise false
*/
bool CommandSettings::reset()
{
// 	disconnect ( ui.twCommand, SIGNAL ( itemChanged ( QTableWidgetItem * ) ), this, SLOT ( checkAndAddCommandValues() ) );
	int currRow = ui.twCommand->currentRow();
	if ( currRow>=0 )
	{
		if ( !allCommandValuesSet ( currRow ) )
		{
			ui.pbImportProgram->setChecked ( false );
			return false;
		}
		if ( commandEdited )
		{
			if ( commandNameExists ( ui.twCommand->item ( currRow, 1 )->text(), currRow ) )
			{
				ui.pbImportProgram->setChecked ( false );
				return false;
			}
			if ( commandValueExists ( ui.twCommand->item ( currRow, 3 )->text(), currRow ) )
			{
				ui.pbImportProgram->setChecked ( false );
				return false;
			}
		}
		//deactivateAllCbs();
	}
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount ( 0 );
	commandEdited = false;
	return init();
}

/**
* \brief Shows only the commands, which have the selected type of the combobox cbShowCommand.
*
* \author Susanne Tschernegg
*/
void CommandSettings::showOnlyCommands()
{
	if ( commandEdited )
	{
		if ( ui.cbShowCommand->currentIndex() !=0 )
		{
			ui.cbShowCommand->setCurrentIndex ( 0 );
			return;
		}

		if ( ! ( ui.twCommand->currentRow() <0 ) )
		{
			int currRow = ui.twCommand->currentRow();

			if ( currRow!=0 )
				ui.twCommand->setCurrentCell ( 0,0 );
			else if ( ui.twCommand->rowCount() >1 )
				ui.twCommand->setCurrentCell ( 1,0 );
		}
		return;
	}

	int currInd = ui.cbShowCommand->currentIndex();
	if ( currInd!=0 )
		ui.leSearchCommand->clear();
	ui.cbShowCommand->setCurrentIndex ( currInd );
	int currRow = ui.twCommand->currentRow();
	if ( currRow>=0 )
	{
		if ( !allCommandValuesSet ( currRow ) )
		{
			return;
		}
		if ( commandEdited )
		{
			if ( commandNameExists ( ui.twCommand->item ( currRow, 1 )->text(), currRow ) )
			{
				return;
			}
			if ( commandValueExists ( ui.twCommand->item ( currRow, 3 )->text(), currRow ) )
			{
				return;
			}
		}
		deactivateAllCbs();
	}

	QString currType = ui.cbShowCommand->currentText();
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount ( 0 );
	QTableWidgetItem *tmp;
	CommandList commands = commandBackend->getCommands();
	int counter = 0;
	for ( int i=0; i < commands.count(); i++ )
	{
		CommandType ctype = commands.at ( i )->getType();
		QString strType = getTypeName ( ctype );

		if ( ( strType==currType ) || ( currType=="Alles" ) )
		{
			counter ++;
			ui.twCommand->setRowCount ( counter );

			QString resourceId = commands.at ( i )->getIconPath();

			QIcon icon = getIconFromResource ( resourceId );

			IconButton *iconButton = new IconButton ( this );
			iconButton->setIcon ( icon );
			iconButton->setIconName ( resourceId );

			ui.twCommand->setCellWidget ( counter-1, 0, iconButton );

			tmp = new QTableWidgetItem ( commands.at ( i )->getName() );
			tmp->setData ( Qt::UserRole, commands.at ( i )->getName() );
			ui.twCommand->setItem ( counter-1, 1, tmp );

			tmp = new QTableWidgetItem ( strType );
			ui.twCommand->setItem ( counter-1, 2, tmp );

			tmp = new QTableWidgetItem ( commands.at ( i )->getValue() );
			ui.twCommand->setItem ( counter-1, 3, tmp );
		}
	}
	ui.twCommand->resizeColumnToContents ( 2 );
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
* \brief Searches for commands which includes the text and the chosen type.
*
* \author Susanne Tschernegg
*/
void CommandSettings::searchCommandList()
{
	if ( ( ui.leSearchCommand->text() =="" ) && ( !commandEdited ) )
	{
		CommandList allCommands = commandBackend->getCommands();
		ui.twCommand->clearContents();
		ui.twCommand->setRowCount ( 0 );
		QTableWidgetItem *tmp;
		int counter = 0;
		for ( int z=0; z<allCommands.count(); z++ )
		{
			CommandType ctype = allCommands.at ( z )->getType();
			QString strType = getTypeName ( ctype );

			if ( ( ui.cbShowCommand->currentIndex() ==0 ) || ( strType==ui.cbShowCommand->currentText() ) )
			{
				counter++;
				ui.twCommand->setRowCount ( counter );

				QString resourceId = allCommands.at ( z )->getIconPath();

				QIcon icon = getIconFromResource ( resourceId );

				IconButton *iconButton = new IconButton ( this );
				iconButton->setIcon ( icon );
				iconButton->setIconName ( resourceId );

				ui.twCommand->setCellWidget ( counter-1, 0, iconButton );

				tmp = new QTableWidgetItem ( allCommands.at ( z )->getName() );
				tmp->setData ( Qt::UserRole, allCommands.at ( z )->getName() );
				ui.twCommand->setItem ( counter-1, 1, tmp );

				CommandType ctype = allCommands.at ( z )->getType();
				QString strType = getTypeName ( ctype );

				tmp = new QTableWidgetItem ( strType );
				ui.twCommand->setItem ( counter-1, 2, tmp );

				tmp = new QTableWidgetItem ( allCommands.at ( z )->getValue() );
				ui.twCommand->setItem ( counter-1, 3, tmp );
			}
		}
		ui.twCommand->resizeColumnToContents ( 1 );
		return;
	}

	int prevRow = ui.twCommand->currentRow();

	if ( ui.leSearchCommand->text() !="" )
		if ( ui.twCommand->rowCount() >0 && prevRow!=0 )
			ui.twCommand->setCurrentCell ( 1,0 );

		else if ( ui.twCommand->rowCount() >1 )
			ui.twCommand->setCurrentCell ( 0,0 );

	if ( !ui.twCommand->item ( prevRow,1 ) )
	{
		if ( !prevRow>=ui.twCommand->rowCount() )
		{
			return;
		}
	}
	else
	{
		if ( ( ui.twCommand->item ( prevRow, 1 )->text() ==NULL ) || ( ui.twCommand->item ( prevRow, 1 )->text() ==NULL ) ||
		        ( ui.twCommand->item ( prevRow,1 )->text().trimmed() =="" ) || ( ui.twCommand->item ( prevRow,3 )->text().trimmed() =="" ) )
		{
			commandEdited = true;
			ui.leSearchCommand->clear();
			return;
		}
	}

	QString searchText = ui.leSearchCommand->text();
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount ( 0 );
	QTableWidgetItem *tmp;
	CommandList commands = commandBackend->getCommands();
	int counter = 0;
	for ( int i=0; i < commands.count(); i++ )
	{
		QString name = commands.at ( i )->getName();
		if ( name.contains ( searchText,Qt::CaseInsensitive ) )
		{
			CommandType ctype = commands.at ( i )->getType();
			QString strType = getTypeName ( ctype );

			if ( ( ui.cbShowCommand->currentIndex() ==0 ) || ( strType==ui.cbShowCommand->currentText() ) )
			{
				counter ++;
				ui.twCommand->setRowCount ( counter );

				QString resourceId = commands.at ( i )->getIconPath();

				QIcon icon = getIconFromResource ( resourceId );

				IconButton *iconButton = new IconButton ( this );
				iconButton->setIcon ( icon );
				iconButton->setIconName ( resourceId );

				ui.twCommand->setCellWidget ( counter-1, 0, iconButton );

				tmp = new QTableWidgetItem ( commands.at ( i )->getName() );
				tmp->setData ( Qt::UserRole, commands.at ( i )->getName() );
				ui.twCommand->setItem ( counter-1, 1, tmp );

				tmp = new QTableWidgetItem ( strType );
				ui.twCommand->setItem ( counter-1, 2, tmp );

				tmp = new QTableWidgetItem ( commands.at ( i )->getValue() );
				ui.twCommand->setItem ( counter-1, 3, tmp );
			}
		}
	}
	ui.twCommand->resizeColumnToContents ( 2 );
}

/**
* \brief to open the importprogramwizard (show or hides it)
*
* \author Susanne Tschernegg
*/
void CommandSettings::importNewProgram()
{
	bool checked = ui.pbImportProgram->isChecked();

	ui.twCommand->setCurrentCell ( 0,0 );
	if ( commandEdited )
		return;

	ui.leSearchCommand->clear();

	showAllCommands();

	ui.pbImportPlace->setDisabled ( checked );
	ui.pbNewCommand->setDisabled ( checked );
	ui.pbEditCommand->setDisabled ( checked );
	ui.pbDeleteCommand->setDisabled ( checked );
	ui.leKeyword->setDisabled ( checked );
	ui.leSearchCommand->setDisabled ( checked );
	ui.pbClearSearchCommand->setDisabled ( checked );
	ui.cbShowCommand->setDisabled ( checked );
	ui.twCommand->setDisabled ( checked );

	if ( checked )
	{
		//importProgramWizard = new ImportProgramWizard(this);
		importProgramWizard->show();
	}
	else
		importProgramWizard->hide();
}

/**
* \brief slot: the signal will be emited in the ImportProgramPage or ImportPlacePage, after a command were created.
*
* \author Susanne Tschernegg, Peter Grasch
*
*   @param Command *command
*       command, which should be inserted in the commandlist
*/
void CommandSettings::insertCommand ( Command* command )
{
	int rows = ui.twCommand->rowCount();
	ui.twCommand->insertRow ( rows );

	IconButton *iconButton = new IconButton ( this );
#ifdef __WIN32
	WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
	QIcon icon = windowsResourceHandler->retrieveIcon ( "shell32.dll", 12 );

	iconButton->setIcon ( icon );
	iconButton->setIconName ( "shell32.dll,12" );

#endif
#ifndef __WIN32
	//there might be an icon set
	QString iconsrc = command->getIconPath();
	iconButton->setIconName ( iconsrc );
	iconButton->setIcon ( QIcon ( iconsrc ) );
#endif
	ui.twCommand->setCellWidget ( rows, 0, iconButton );

	QTableWidgetItem *tmp = new QTableWidgetItem ( command->getName() );
	tmp->setData ( Qt::UserRole, command->getName() );
	ui.twCommand->setItem ( rows, 1, tmp );

	CommandType ctype = command->getType();
	QString strType = getTypeName ( ctype );

	ui.twCommand->setItem ( rows, 2, new	QTableWidgetItem ( strType ) );

	ui.twCommand->setItem ( rows, 3, new QTableWidgetItem ( command->getValue() ) );

	ui.twCommand->setItem ( rows, 4, new QTableWidgetItem ( "" ) );

	for ( int i=0; i<ui.twCommand->rowCount(); i++ )
	{
		if ( ( i!=rows ) && ( QString::compare ( command->getName(), ui.twCommand->item ( i, 1 )->text(), Qt::CaseInsensitive ) ==0 ) )
		{
			int result = QMessageBox::information ( this, tr ( "Name existiert bereits" ), tr ( "Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte ändern Sie den Namen des neuen Kommandos.\n\nWollen Sie den Wizard nun verlassen? (Klicken Sie Nein, um den Namen zu ändern!" ),
			                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
			if ( result == QMessageBox::Yes )
			{
				if ( !ui.twCommand->item ( rows, 1 )->data ( Qt::UserRole ).isNull() )
					commandBackend->deleteCommand ( ui.twCommand->item ( rows, 1 )->data ( Qt::UserRole ).toString() );
				ui.twCommand->removeRow ( rows );
				emit changeExistingName ( false );
			}
			else
			{
				if ( !ui.twCommand->item ( rows, 1 )->data ( Qt::UserRole ).isNull() )
					commandBackend->deleteCommand ( ui.twCommand->item ( rows, 1 )->data ( Qt::UserRole ).toString() );
				ui.twCommand->removeRow ( rows );
				emit changeExistingName ( true );
			}
		}
	}
}


/**
* \brief If a commandname already exists, a messagebox will be opend and you can either change the name or the command will be deleted.
*
* \author Susanne Tschernegg
*
*   @param QString name
*       the name of the command
*   @param int prevRow
*       the previous row
*   @return bool
*       returns true if the name of the command already exists, otherwise false
*/
bool CommandSettings::commandNameExists ( QString name, int prevRow )
{
	for ( int i=0; i<ui.twCommand->rowCount(); i++ )
	{
		if ( ( i!=prevRow ) && ( !ui.twCommand->item ( i, 1 )->data ( Qt::UserRole ).isNull() ) && ( QString::compare ( name, ui.twCommand->item ( i, 1 )->data ( Qt::UserRole ).toString(), Qt::CaseInsensitive ) ==0 ) )
		{
			int result = QMessageBox::information ( this, tr ( "Name existiert bereits" ), tr ( "Dieser Name wurde bereits für ein anderes Kommando verwendet!\nBitte geben Sie einen anderen Namen für dieses Kommando ein.\n\nWollen Sie das Kommando löschen? (Klicken Sie Nein, um den Namen jetzt zu ändern)" ),
			                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
			if ( result == QMessageBox::Yes )
			{
				if ( !ui.twCommand->item ( prevRow, 0 )->data ( Qt::UserRole ).isNull() )
				{
					commandBackend->deleteCommand ( ui.twCommand->item ( prevRow, 1 )->data ( Qt::UserRole ).toString() );
				}
				commandEdited=false;
				ui.twCommand->removeRow ( prevRow );
			}
			else
			{
				ui.twCommand->selectColumn ( 1 );
				ui.twCommand->selectRow ( prevRow );
				ui.twCommand->setCurrentCell ( prevRow,1 );
				ui.twCommand->editItem ( ui.twCommand->item ( prevRow,1 ) );
				commandEdited=true;
				ui.pbImportProgram->setChecked ( false );
				ui.pbImportPlace->setChecked ( false );
				return true;
			}
		}
	}
	return false;
}

/**
* \brief If the value of an command already exists, the users attention will be called and the user can change the value or the command will be deleted.
*
* \author Susanne Tschernegg
*
*   @param QString value
*       holds the value of the command
*   @param int prevRow
*       previous row
*   @return bool
*       returns true when the value of an command already exists otherwise false
*/
bool CommandSettings::commandValueExists ( QString value, int prevRow )
{
	for ( int i=0; i<ui.twCommand->rowCount(); i++ )
	{
		if ( ( i!=prevRow ) && ( QString::compare ( value, ui.twCommand->item ( i, 3 )->text(), Qt::CaseInsensitive ) ==0 ) )
		{
			int result = QMessageBox::critical ( this, tr ( "Wert existiert bereits" ), tr ( "Dieser Wert existiert bereits!\n\nWollen Sie den Wert jetzt ändern? (Klickes Sie Nein, um den Wert jetzt zu behalten)" ),
			                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );
			if ( result == QMessageBox::Yes )
			{
				ui.twCommand->selectColumn ( 3 );
				ui.twCommand->selectRow ( prevRow );
				ui.twCommand->editItem ( ui.twCommand->item ( prevRow, 3 ) );
				ui.twCommand->setCurrentCell ( prevRow,1 );
				commandEdited=true;
				ui.pbImportProgram->setChecked ( false );
				ui.pbImportPlace->setChecked ( false );
				return true;
			}
		}
	}
	return false;
}

/**
* \brief Checks, whether all values of a comment were set, or if the value isn't reasonable.
*
* \author Susanne Tschernegg
*   @param int prevRow
*       holds the previous row
*   @return bool
*       returns if all values of an command are valid
*/
bool CommandSettings::allCommandValuesSet ( int prevRow )
{
	if ( prevRow>=ui.twCommand->rowCount() || prevRow<0 )
		return true;
	if ( ( ui.twCommand->item ( prevRow, 1 )->text() ==NULL ) || ( ui.twCommand->item ( prevRow, 1 )->text() ==NULL ) ||
	        ( ui.twCommand->item ( prevRow,1 )->text().trimmed() =="" ) || ( ui.twCommand->item ( prevRow,3 )->text().trimmed() =="" ) )
	{
		int result = QMessageBox::question ( this, tr ( "Leeres Kommandofeld" ), tr ( "Dieses Kommando wurde nicht vollständig ausgef?lt.\nJedes Kommando muss einen Namen, Wert und Typ besitzen.\n\nWollen Sie diesen Eintrag jetzt löschen? (Klicken Sie Nein, um das Kommando jetzt zu vervollständigen)" ),
		                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

		if ( result == QMessageBox::Yes )
		{
			if ( ( !ui.twCommand->item ( prevRow, 1 )->data ( Qt::UserRole ).isNull() ) && ( ( ui.twCommand->item ( prevRow, 0 )->data ( Qt::UserRole ).toString().trimmed() !="" ) ) )
			{
				//deletes the command by the data
				commandBackend->deleteCommand ( ui.twCommand->item ( prevRow, 1 )->data ( Qt::UserRole ).toString() );
			}
			commandEdited=false;
			ui.twCommand->removeRow ( prevRow );
		}
		else
		{
			ui.twCommand->selectColumn ( 1 );
			ui.twCommand->selectRow ( prevRow );
			ui.twCommand->editItem ( ui.twCommand->item ( prevRow, 1 ) );
			ui.twCommand->setCurrentCell ( prevRow,1 );
			commandEdited=true;
			ui.pbImportProgram->setChecked ( false );
			ui.pbImportPlace->setChecked ( false );
		}
		return false;
	}
	return true;
}

/**
* \brief Deletes the combobox and writes instead the type as plain text into the cell.
*
* \author Susanne Tschernegg
*   @param int prevRow
*       holds the previous row
*/
void CommandSettings::deactivateCB ( int prevRow )
{
	QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget ( prevRow, 2 );
	if ( tmpWidget!=NULL )
	{
		QComboBox *cbType = ( QComboBox* ) tmpWidget;
		QString type = cbType->itemText ( cbType->currentIndex() );
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText ( type );
		ui.twCommand->removeCellWidget ( prevRow, 2 );
		ui.twCommand->setItem ( prevRow, 2, tmp );

		int typeInt = getTypeNumber ( type );

		//creates esp. replaces a command
		Command *newCommand = new Command ( ui.twCommand->item ( prevRow,1 )->text(), CommandType ( typeInt ), ui.twCommand->item ( prevRow,3 )->text() );
		if ( ( !ui.twCommand->item ( prevRow,1 )->data ( Qt::UserRole ).isNull() ) && ( commandBackend->commandExists ( ui.twCommand->item ( prevRow,1 )->data ( Qt::UserRole ).toString() ) ) )
		{
			//replaces the command
			commandBackend->replaceCommand ( ui.twCommand->item ( prevRow,1 )->data ( Qt::UserRole ).toString(), newCommand );
			ui.twCommand->item ( prevRow,1 )->setData ( Qt::UserRole, ui.twCommand->item ( prevRow,1 )->text() );
		}
		else
		{
			commandBackend->addCommand ( newCommand );
			ui.twCommand->item ( prevRow,1 )->setData ( Qt::UserRole, ui.twCommand->item ( prevRow,1 )->text() );
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
	ui.pbImportProgram->setChecked ( false );
	ui.pbImportPlace->setChecked ( false );

	ui.pbImportProgram->setDisabled ( false );
	ui.pbImportPlace->setDisabled ( false );
	ui.pbNewCommand->setDisabled ( false );
	ui.pbEditCommand->setDisabled ( false );
	ui.pbDeleteCommand->setDisabled ( false );
	ui.leKeyword->setDisabled ( false );
	ui.leSearchCommand->setDisabled ( false );
	ui.pbClearSearchCommand->setDisabled ( false );
	ui.cbShowCommand->setDisabled ( false );
	ui.twCommand->setDisabled ( false );
}

/**
* \brief shows all existing commands
*
* \author Susanne Tschernegg
*/
void CommandSettings::showAllCommands()
{
	ui.cbShowCommand->setCurrentIndex ( 0 );
}

/**
* \brief Checks if every command value is valid.
*
* \author Susanne Tschernegg
*/
void CommandSettings::checkValuesAfterReturnPressed()
{
	int currRow = ui.twCommand->currentRow();
	int currCol = ui.twCommand->currentColumn();
	if ( currRow!=0 )
		ui.twCommand->setCurrentCell ( 0,0 );
	else if ( ui.twCommand->rowCount() >1 )
		ui.twCommand->setCurrentCell ( 1,0 );
	if ( currRow<ui.twCommand->rowCount() )
		ui.twCommand->setCurrentCell ( currRow,currCol );
	deactivateCB ( currRow );
	commandEdited = false;
}

/**
* \brief Shows or hides the importNewPlace-Wizard depending on wheter it is shown yet or not.
*
* \author Susanne Tschernegg
*/
void CommandSettings::importNewPlace()
{
	bool checked = ui.pbImportPlace->isChecked();
	ui.twCommand->setCurrentCell ( 0,0 );
	if ( commandEdited )
	{
		return;
	}
	ui.leSearchCommand->clear();

	showAllCommands();

	ui.pbImportProgram->setDisabled ( checked );
	ui.pbNewCommand->setDisabled ( checked );
	ui.pbEditCommand->setDisabled ( checked );
	ui.pbDeleteCommand->setDisabled ( checked );
	ui.leKeyword->setDisabled ( checked );
	ui.leSearchCommand->setDisabled ( checked );
	ui.pbClearSearchCommand->setDisabled ( checked );
	ui.cbShowCommand->setDisabled ( checked );
	ui.twCommand->setDisabled ( checked );

	if ( checked )
	{
		//importPlaceWizard = new ImportPlaceWizard(this);
		importPlaceWizard->show();
	}
	else
		importPlaceWizard->hide();
}

/**
* \brief Returns the icon from the given resourceId.
*
* \author Susanne Tschernegg
*   @param QString resourceId
*       holds the name of the resource
*   @return QIcon
*       icon from the resourceId
*/
QIcon CommandSettings::getIconFromResource ( QString resourceId )
{
	QIcon icon;
#ifdef __WIN32
	if ( resourceId.contains ( QRegExp ( ".dll,\n*" ) ) )
	{
		QStringList iconResources = resourceId.split ( "," );
		WindowsResourceHandler *windowsResourceHandler = new WindowsResourceHandler();
		icon = windowsResourceHandler->retrieveIcon ( iconResources.at ( 0 ), iconResources.at ( 1 ).toInt() );
	}
	else
	{
		QPixmap pixmap ( resourceId );
		icon.addPixmap ( pixmap );
	}
#endif
#ifndef __WIN32
	QPixmap pixmap ( resourceId );
	icon.addPixmap ( pixmap );
#endif
	return icon;
}

/**
* \brief Returns the name of the given type.
*
* \author Susanne Tschernegg
*   @param CommandType ctype
*       type of an command
*   @return QString
*       returns the name of the given type
*/
QString CommandSettings::getTypeName ( CommandType ctype )
{
	QString strType = "";
	if ( ctype == place )
		strType = tr ( "Orte" );
	else if ( ctype == type )
		strType = tr ( "Sonderzeichen" );
	else strType = tr ( "Programme" );
	return strType;
}

/**
* \brief Returns the number of the given typename.
*
* \author Susanne Tschernegg
*   @param QString commandName
*       holds the name of the command
*   @return int
*       returns the number of the given typename
*/
int CommandSettings::getTypeNumber ( QString commandName )
{
	int typeInt;
	if ( commandName == "Orte" )
		typeInt = 1;
	else if ( commandName == "Sonderzeichen" )
		typeInt = 2;
	else typeInt = 0;
	return typeInt;
}
