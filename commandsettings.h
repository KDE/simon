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
#ifdef __WIN32
#include "registrymanager.h"
#endif

class ImportProgramWizard;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class CommandSettings : public SystemWidget
{
    Q_OBJECT
public:
    CommandSettings(QWidget* parent);
    ~CommandSettings();


    //void editCommands();

    bool init();
    bool apply();
    bool reset();
    void deactivateAllCbs();

private slots:
	void newCommand();
	void deleteCommand();
	void activateCb();
	void checkAndAddCommandValues(int currRow, int currCol, int prevRow, int prevCol);
	void editCommand();
	void showOnlyCommands();
    void clearSearchLineEdit();
    void searchCommandList();
    void importNewProgram();

public slots:
    void insertCommand(Command *command);

private:
    Ui::CommandSettingsDlg ui;

    ImportProgramWizard* importProgramWizard;
    int commandsCount;

    bool commandNameExists(QString name, int prevRow);
    bool commandValueExists(QString value, int prevRow);
    bool allCommandValuesSet(int prevRow);

    void addCommand(int prevRow);

    XMLCommand *commandLoader;

signals:
    void commandsChanged();
};

#endif
