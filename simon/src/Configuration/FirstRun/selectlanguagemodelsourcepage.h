//
// C++ Interface: selectlanguagemodelsourcepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTLANGUAGEMODELSOURCEPAGE_H
#define SELECTLANGUAGEMODELSOURCEPAGE_H

#include <QWizardPage>
#include "ui_selectlanguagemodelsource.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class SelectLanguageModelSourcePage : public QWizardPage
{
Q_OBJECT
private:
	Ui::LanguageModelSourcePg ui;
private slots:
	void updateInfo();
public:
    SelectLanguageModelSourcePage(QWidget* parent);
    bool isComplete() const { return ui.rbNewModel->isChecked() || ui.rbOpenModel->isChecked(); }
    int nextId() const;

    ~SelectLanguageModelSourcePage();

};

#endif
