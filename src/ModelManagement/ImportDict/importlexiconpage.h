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
private:
	Ui::ImportDictLexiconPage ui;
public:
    ImportLexiconPage(QWidget* parent);

    ~ImportLexiconPage();

};

#endif
