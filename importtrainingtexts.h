//
// C++ Interface: importtrainingtexts
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTTRAININGTEXTS_H
#define IMPORTTRAININGTEXTS_H

#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QProgressBar>
#include <QPushButton>
#include "quickdownloader.h"
#include "xmltrainingtextlist.h"
#include "importlocalwizardpage.h"
#include "importremotewizardpage.h"
#include "importworkingwizardpage.h"
#include "selectsourcewizardpage.h"
#include "xmltrainingtext.h"

/**
 * \brief Guides the user through the process of adding a new trainingtext
 * 
 * This class shows a Wizard that walks the user through that process
 * 
 * \author Peter Grasch
 * \date 20.05.2007
 * \version 0.1
*/
class ImportTrainingTexts : public QWizard{
	
	Q_OBJECT
private:
	QFileDialog *fd;
	int prevId;
public:
    ImportTrainingTexts(QWidget *parent=0);
    
    void start();
    
    
    //creating the pages for the wizard

    QWizardPage* createIntroPage();
    QWizardPage* createSourcePage();
    QWizardPage* createRemoteImportPage();
    QWizardPage* createLocalImportPage();
    QWizardPage* createWorkingPage();
    QWizardPage* createFinishedPage();

    ~ImportTrainingTexts();

public slots:
	void idChanged(int id);

};

#endif
