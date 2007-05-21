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

#include <QWidget>
#include <QWizard>
#include <QWizardPage>
#include <QMessageBox>

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
	
	enum { PageIntro, PageSource, PageFile, PageInternet, PageWorking, PageFinished };
	
public:
    ImportTrainingTexts(QWidget *parent=0);
    
    void start();
    
    
    //creating the pages for the wizard
    QWizardPage* createIntroPage();
    QWizardPage* createSourcePage();
    QWizardPage* createWorkingPage();
    QWizardPage* createFinishedPage();

    ~ImportTrainingTexts();

};

#endif
