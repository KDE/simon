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


#ifndef IMPORTDICTWIKTIONARYPAGE_H
#define IMPORTDICTWIKTIONARYPAGE_H

#include <QWizardPage>
#include "importdictview.h"
#include "ui_importwiktionarypage.h"

class SimonListWidget;
class QLineEdit;
class QPushButton;
class QLabel;

/**
 * \class ImportDictWiktionaryPage
 * \brief Lets the user select a local or remote wiktionary to import
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
 */
class ImportDictWiktionaryPage : public QWizardPage
{
Q_OBJECT

private:
	SimonListWidget *remoteList;
	QLineEdit *leWikiPath;
	QPushButton *pbWikiSelectFolder;
	QLabel *lbWikiPath;
	Ui::ImportWiktionaryDlg ui;
	
private slots:
    void resambleImportLocal(bool isTru);
    void loadList();
    void importList(QString list);


public:
    ImportDictWiktionaryPage(QWidget* parent);
	bool isComplete() const;
	virtual int nextId() const { return ImportDictView::WorkingPage; }

    ~ImportDictWiktionaryPage();

};

#endif
