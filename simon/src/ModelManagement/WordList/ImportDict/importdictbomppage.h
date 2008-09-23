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
#ifndef IMPORTDICTBOMPPAGE_H
#define IMPORTDICTBOMPPAGE_H

#include <QWizardPage>
#include "importdictview.h"
#include "ui_importdictbomppage.h"
class QString;

/**
 * \class ImportDictBOMPPage
 * \brief WizardPage to select a BOMP Dictionary
 * \author Peter Grasch
 * \date 10.8.2007
 * \version 0.1
 */
class ImportDictBOMPPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ImportBOMPDlg ui;
public:
    ImportDictBOMPPage(QWidget* parent);
	int nextId() const { return ImportDictView::WorkingPage; }

    ~ImportDictBOMPPage();

};

#endif
