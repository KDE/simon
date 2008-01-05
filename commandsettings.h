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
#include "xmlcommand.h"
#include <QStringList>
#include "simontablewidget.h"
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

/**
	@author Peter Grasch <bedahr@gmx.net>
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
		SimonTableWidget *twCommand;

		ImportProgramWizard* importProgramWizard;
		ImportPlaceWizard* importPlaceWizard;
		int commandsCount;

		bool commandNameExists ( QString name, int prevRow );
		bool commandValueExists ( QString value, int prevRow );
		bool allCommandValuesSet ( int prevRow );

		void showAllCommands();

		void deactivateCB ( int prevRow );

		XMLCommand *commandLoader;
		bool commandEdited;
		QIcon getIconFromResource ( QString resourceId );
		QString getTypeName ( CommandType ctype );
		int getTypeNumber ( QString commandName );

	signals:
		void commandsChanged();
		void changeExistingName ( bool changeName );
};

#endif
