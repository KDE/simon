//
// C++ Interface: addwordrecordpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ADDWORDRECORDPAGE_H
#define ADDWORDRECORDPAGE_H

#include <QWizardPage>
#include "recwidget.h"

/**
 \class AddWordRecordPage
 \author Peter Grasch
 \date 01.08.2007
 \brief Records two samples of the new word
 
 The WizardPage of the AddWordView-Wizard that manages the recordings;
 Uses two Recwidgets and the QT-Mechanism to ensure that you can't 
 continiue without both recordings done.

 Trys to determine useful sentences by using the grammarmanager and
 let the user record them
*/
class AddWordRecordPage : public QWizardPage
{
private:
	RecWidget *rec1, *rec2; ///<! The 
public:
    void initializePage();
    AddWordRecordPage(QWidget *parent=0);
    bool isComplete() const 
    { return rec1 && rec2 && rec1->hasRecordingReady() && rec2->hasRecordingReady(); }
    void cleanUp();

    ~AddWordRecordPage();

};

#endif
