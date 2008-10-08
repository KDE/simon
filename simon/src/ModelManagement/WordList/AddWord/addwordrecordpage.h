/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef ADDWORDRECORDPAGE_H
#define ADDWORDRECORDPAGE_H

#include <QVBoxLayout>
#include <QWizardPage>
#include "../../../SimonLib/sound/recwidget.h"
#include "ui_addwordrecordpage.h"

/**
 \class AddWordRecordPage
 \author Peter Grasch
 \date 01.08.2007
 \brief Records two samples of the new word
 
 The WizardPage of the AddWordView-Wizard that manages the recordings;
 Uses two Recwidgets and the QT-Mechanism to ensure that you can't 
 continiue without both recordings done.

 Tries to determine useful sentences by using the grammarmanager and
 let the user record them
*/
class AddWordRecordPage : public QWizardPage
{
    Q_OBJECT
    
signals:
	void recordingNamesGenerated(QString,QString);
private:
	RecWidget *rec1, *rec2; ///<! The recwidgets
	QString getSamplesDir();
	Ui::AddWordRecordPage ui;

public:
    void initializePage();
    AddWordRecordPage(QWidget *parent=0);
    
    bool isComplete() const 
    {
// 	return true;
	//make recordings mandatory
	return rec1 && rec2 && rec1->hasRecordingReady() && rec2->hasRecordingReady(); 
    }

    ~AddWordRecordPage();

};

#endif
