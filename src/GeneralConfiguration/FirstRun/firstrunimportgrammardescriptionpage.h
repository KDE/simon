//
// C++ Interface: firstrunimportgrammardescriptionpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNIMPORTGRAMMARDESCRIPTIONPAGE_H
#define FIRSTRUNIMPORTGRAMMARDESCRIPTIONPAGE_H

#include <QWizardPage>
#include "ui_importgrammardescriptionpage.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class FirstRunImportGrammarDescriptionPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ImportGrammarDescriptionPg ui;
public:
    FirstRunImportGrammarDescriptionPage(QWidget* parent);

	int nextId() const;

    ~FirstRunImportGrammarDescriptionPage();

};

#endif
