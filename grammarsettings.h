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

class GrammarSettings : public SystemWidget
{
Q_OBJECT
private:
	ImportGrammarWizard *importGrammarWizard;
	MergeTerminalsWizard *mergeTerminalsWizard;
	Ui::GrammarSettingsDlg ui;
	GrammarManager *grammarManager;

	QStringList getCurrentStructures();
	void insertSentences(QStringList sentences);

private slots:
	void sentenceSelectionChanged(int row, int col, int oldrow, int oldcol);
	void deleteSelectedSentence();
	void addSentence();
	void showImportWizard(bool show);
	void showMergeWizard(bool show);
	void mergeGrammar(QStringList);
public slots:
	bool init();
	bool apply();
	bool reset();
public:
    GrammarSettings(QWidget* parent, GrammarManager *grammarManager);

    ~GrammarSettings();

};

#endif
