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


#ifndef SIMON_IMPORTDICTBOMPPAGE_H_9F203AF0D54744DA92777EE86A3573DA
#define SIMON_IMPORTDICTBOMPPAGE_H_9F203AF0D54744DA92777EE86A3573DA

#include <QWizardPage>
#include "importdictview.h"
#include "ui_importdictbomppage.h"
class QString;

/**
 * \class ImportDictBOMPPage
 * \brief WizardPage to select a BOMP Dictionary
 * \author Peter Grasch
 * \date 10.8.2007
 * \version 0.1
 */
class ImportDictBOMPPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ImportBOMPDlg ui;
public:
    ImportDictBOMPPage(QWidget* parent);
	int nextId() const;
	bool isComplete() const;
	void initializePage();

    ~ImportDictBOMPPage();

};

#endif
