//
// C++ Interface: importgrammarselectfilespage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTGRAMMARSELECTFILESPAGE_H
#define IMPORTGRAMMARSELECTFILESPAGE_H

#include <QWizardPage>
#include "ui_selectfilestoreadpage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportGrammarSelectFilesPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::SelectFilesPage ui;
public:
    ImportGrammarSelectFilesPage(QWidget* parent);
	void cleanupPage();
    ~ImportGrammarSelectFilesPage();

};

#endif
