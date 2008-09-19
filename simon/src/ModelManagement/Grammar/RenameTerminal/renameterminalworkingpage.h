//
// C++ Interface: renameterminalworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENAMETERMINALWORKINGPAGE_H
#define RENAMETERMINALWORKINGPAGE_H

#include <QWizardPage>
#include "ui_renameterminalworkingpage.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class RenameTerminal;
class RenameTerminalWorkingPage : public QWizardPage
{
Q_OBJECT
signals:
	void done();
private slots:
	void finish();
	void displayProgress(int);
private:
	Ui::RenameTerminalWorkingPage ui;
	bool complete;
	RenameTerminal *renameTerminal;

public:
    RenameTerminalWorkingPage(QWidget *parent);

	void initializePage();
	bool isComplete() const { return this->complete; }

    ~RenameTerminalWorkingPage();

};

#endif
