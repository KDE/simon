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


#ifndef SIMON_IMPORTGRAMMARWORKINGPAGE_H_038AD3EAE27749BAA1F38A6BAEE5243E
#define SIMON_IMPORTGRAMMARWORKINGPAGE_H_038AD3EAE27749BAA1F38A6BAEE5243E

#include <QWizardPage>
#include "ui_importgrammarworkingpage.h"
#include <simonscenarios/word.h>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportGrammar;
class ImportGrammarWorkingPage : public QWizardPage
{
Q_OBJECT
signals:
	void grammarCreated();
private:
	bool completed;
	Ui::ImportGrammarWorkingPage ui;
	ImportGrammar *grammarImporter;

public slots:
	void cancel();

private slots:
	void initializePage();
	void printStatus(QString);
	void displayFileProgress(int progress, int max);
	void displayWholeProgress(int progress, int max);
	void processCompletion();

public:
    ImportGrammarWorkingPage(QWidget* parent);

	bool isComplete() const { return completed; }
    ~ImportGrammarWorkingPage();

};

#endif
