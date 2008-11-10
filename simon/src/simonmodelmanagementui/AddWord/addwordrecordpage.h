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

#include <QWizardPage>

class TrainSamplePage;

/**
 \class AddWordRecordPage
 \author Peter Grasch
 \date 01.08.2007
 \brief Records two samples of the new word
 
 The WizardPage of the AddWordView-Wizard that manages the recordings;
 Uses two Recwidgets and the QT-Mechanism to ensure that you can't 
 continiue without both recordings done.
*/
class AddWordRecordPage : public QWizardPage
{
    Q_OBJECT
    
signals:
	void recordingNamesGenerated(QString,QString);
private:
	TrainSamplePage *page;
	int pageNr, pageMax;
	QString fieldName;

public:
	QString getFileName();
	QString getPrompt();

public:
    void initializePage();
    AddWordRecordPage(const QString& fieldName, int pageNr, int pageMax, QWidget *parent=0);
    
    bool isComplete() const;

    ~AddWordRecordPage();

};

#endif
