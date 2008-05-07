//
// C++ Interface: importbomppage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTBOMPPAGE_H
#define IMPORTBOMPPAGE_H

#include <QWizardPage>
#include "importdictview.h"
#include "ui_importbomppage.h"
class QString;

/**
 * \class ImportBOMPPage
 * \brief WizardPage to select a BOMP Dictionary
 * \author Peter Grasch
 * \date 10.8.2007
 * \version 0.1
 */
class ImportBOMPPage : public QWizardPage
{
	Ui::ImportBOMPDlg ui;
public:
    ImportBOMPPage(QWidget* parent);
	int nextId() const { return ImportDictView::WorkingPage; }

    ~ImportBOMPPage();

};

#endif
