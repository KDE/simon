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
	@author Peter Grasch <bedahr@gmx.net>
*/
class AddWordRecordPage : public QWizardPage
{
private:
	RecWidget *rec1, *rec2;
public:
    AddWordRecordPage(QWidget *parent=0);
    bool isComplete() const 
    { return rec1 && rec2 && rec1->hasRecordingReady() && rec2->hasRecordingReady(); }
    void cleanUp();

    ~AddWordRecordPage();

};

#endif
