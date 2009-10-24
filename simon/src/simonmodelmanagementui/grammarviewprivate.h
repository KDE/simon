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


#ifndef GRAMMARVIEWPRIVATE_H
#define GRAMMARVIEWPRIVATE_H

#include "ui_grammarview.h"
#include <QTimer>
#include <speechmodelmanagement/scenariodisplay.h>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class QSortFilterProxyModel;

class GrammarViewPrivate : public QWidget, public ScenarioDisplay 
{
Q_OBJECT
private:
	Ui::GrammarView ui;
	QTimer autoSaveTimer;
	QSortFilterProxyModel *grammarProxy;

private slots:
	void showRenameWizard();
	void showImportWizard();
	void showMergeWizard();
	
	void currentSelectionChanged();

	void addStructure();
	void deleteStructure();
//	void slotChanged();

public slots:
	void displayScenarioPrivate(Scenario *scenario);

public:
    GrammarViewPrivate(QWidget* parent);

    ~GrammarViewPrivate();

};

#endif
