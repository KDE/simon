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


#ifndef IMPORTDICTSPHINXPAGE_H
#define IMPORTDICTSPHINXPAGE_H

#include <QWizardPage>
#include "importdictview.h"
#include "ui_importdictsphinxpage.h"
class QString;

/**
 * \class ImportDictSPHINXPage
 * \brief WizardPage to select a SPHINX Dictionary
 * \author Peter Grasch
 * \date 10.8.2007
 * \version 0.1
 */
class ImportDictSPHINXPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ImportSPHINXDlg ui;
public:
    ImportDictSPHINXPage(QWidget* parent);
	int nextId() const { return ImportDictView::WorkingPage; }

    ~ImportDictSPHINXPage();

};

#endif
