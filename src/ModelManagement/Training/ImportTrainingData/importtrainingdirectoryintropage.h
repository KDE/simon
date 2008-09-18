//
// C++ Interface: importtrainingdirectoryintropage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGDIRECTORYINTROPAGE_H
#define IMPORTTRAININGDIRECTORYINTROPAGE_H

#include <QWizardPage>
#include <QVariant>
#include "ui_importtrainingdataintrodlg.h"


/**
  \class ImportTrainingDirectoryIntroPage
  \author Peter Grasch
  \brief Shows the user an introduction and asks for the folder
  
  Extends QWizardPage and uses the QT-Mandatory-field mechanism
*/
class ImportTrainingDirectoryIntroPage : public QWizardPage{
	Q_OBJECT

private:
	Ui::ImportTrainingDataIntroDlg ui;
public:
	ImportTrainingDirectoryIntroPage(QWidget *parent=0);
	~ImportTrainingDirectoryIntroPage() {}

};

#endif
