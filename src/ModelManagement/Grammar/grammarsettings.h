//
// C++ Interface: grammarsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GRAMMARSETTINGS_H
#define GRAMMARSETTINGS_H

#include "systemwidget.h"
#include "ui_grammarsettingsdlg.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class GrammarManager;
class ImportGrammarWizard;
class MergeTerminalsWizard;
class RenameTerminalWizard;

class GrammarSettings : public SystemWidget
{
Q_OBJECT
private:
	ImportGrammarWizard *importGrammarWizard;
	RenameTerminalWizard *renameTerminalWizard;
	MergeTerminalsWizard *mergeTerminalsWizard;
	Ui::GrammarSettingsDlg ui;

	QStringList getCurrentStructures();
	void insertSentences(QStringList sentences);
	QStringList getCurrentTerminals();


private slots:
	void showRenameWizard(bool show);
	void showImportWizard(bool show);
	void showMergeWizard(bool show);
	void askForSave();
public slots:
	bool init();
	bool apply();
	bool reset();
	bool isComplete();
	void mergeGrammar(QStringList);
public:
    GrammarSettings(QWidget* parent);

    ~GrammarSettings();

};

#endif
