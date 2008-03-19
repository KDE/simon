//
// C++ Interface: commandsettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDSETTINGS_H
#define COMMANDSETTINGS_H

#include "systemwidget.h"
#include "ui_commandsettingsdlg.h"
#include <QMessageBox>
#include <QStringList>
//#include "simontablewidget.h"
#include <QTableWidget>
#ifdef __WIN32
#include "registrymanager.h"
#endif
#include "importplacewizard.h"
class ImportProgramWizard;
class QIcon;
//class ImportPlaceWizard;

#include "qwindowsstyle.h"
#include <QPixmap>

#include "iconbutton.h"
class RunCommand;

/**
 * \class CommandSettings
 * \author Peter Grasch <bedahr@gmx.net>
 * \date 20.08.2007
 * \version 0.1
 * \brief This class holdes self-defined commends, which are programs, places, additional character or macros.
 *      Commands have a name, which identifies the command. Furthermore there is the type of the command (which are above listed).
 *      It also has a value. Commands can also have an Icon, which is placed on an IconButton.
 *      Commands are first read out of a xml-file. The user could change this list. For Example: insert, delete, change etc. commands
 */

class CommandSettings : public SystemWidget
{
		Q_OBJECT
	public:
		CommandSettings ( QWidget* parent );
		~CommandSettings();


		//void editCommands();

		bool init();
		bool apply();
		bool reset();
		bool isComplete();
		void deactivateAllCbs();

	private slots:
		void newCommand();
		void deleteCommand();
		void activateCb();
		void checkAndAddCommandValues ( int currRow, int currCol, int prevRow, int prevCol );
		void editCommand ( int row = -1, int column = 0 );
		void showOnlyCommands();
		void clearSearchLineEdit();
		void searchCommandList();
		void importNewProgram();
		void importNewPlace();

	public slots:
		void insertCommand ( Command *command );
		void setWidgetsDisabled();
		void checkValuesAfterReturnPressed();

	private:
		Ui::CommandSettingsDlg ui;
		QTableWidget *twCommand;

		ImportProgramWizard* importProgramWizard;
		ImportPlaceWizard* importPlaceWizard;
		int commandsCount;

		bool commandNameExists ( QString name, int prevRow );
		bool commandValueExists ( QString value, int prevRow );
		bool allCommandValuesSet ( int prevRow );

		void showAllCommands();

		void deactivateCB ( int prevRow );

		RunCommand *commandBackend;

		bool commandEdited; //!< shows, if a command is edited or not.
		QIcon getIconFromResource ( QString resourceId );
		QString getTypeName ( CommandType ctype );
		int getTypeNumber ( QString commandName );

	signals:
		void commandsChanged();
		void changeExistingName ( bool changeName );
};

#endif
