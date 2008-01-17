//
// C++ Interface: importlexiconpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTLEXICONPAGE_H
#define IMPORTLEXICONPAGE_H

#include <QWizardPage>
#include "ui_importdictlexiconpage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportLexiconPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ImportDictLexiconPage ui;
private slots:
	void selectFile();
public:
    ImportLexiconPage(QWidget* parent);

    ~ImportLexiconPage();

};

#endif
