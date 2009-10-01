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


#ifndef SIMON_IMPORTTRAININGDIRECTORYINTROPAGE_H_C39B92EAEBA04294AF5324392922B89C
#define SIMON_IMPORTTRAININGDIRECTORYINTROPAGE_H_C39B92EAEBA04294AF5324392922B89C

#include <QWizardPage>
#include <QVariant>
#include "ui_importtrainingdataintrodlg.h"


/**
  \class ImportTrainingDirectoryIntroPage
  \author Peter Grasch
  \brief Shows the user an introduction and asks for the folder
  
  Extends QWizardPage and uses the QT-Mandatory-field mechanism
*/
class ImportTrainingDirectoryIntroPage : public QWizardPage{
	Q_OBJECT

private:
	Ui::ImportTrainingDataIntroDlg ui;

private slots:
	void promptsPathChanged();

public:
	ImportTrainingDirectoryIntroPage(QWidget *parent=0);
	~ImportTrainingDirectoryIntroPage() {}
	bool isComplete() const;

};

#endif
