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

#ifndef SIMON_IMPORTDICTBOMPDOWNLOADPAGE_H_2CEF8F0A71564DA294A6B36ED9B82CB1
#define SIMON_IMPORTDICTBOMPDOWNLOADPAGE_H_2CEF8F0A71564DA294A6B36ED9B82CB1

#include <QWizardPage>
#include "importdictview.h"
#include "ui_importdictbompdownloadpage.h"
class QString;

/**
 * \class ImportDictBOMPDownloadPage
 * \brief WizardPage to select a BOMP Dictionary
 * \author Peter Grasch
 * \date 16.9.2009
 * \version 0.1
 */
class ImportDictBOMPDownloadPage : public QWizardPage
{
  Q_OBJECT
    private:
    Ui::ImportBOMPDownloadDlg ui;
    QString bompUrl;
    QString acceptTag;
  public:
    ImportDictBOMPDownloadPage(QWidget* parent);
    int nextId() const { return ImportDictView::WorkingPage; }
    void initializePage();
    bool validatePage();

    ~ImportDictBOMPDownloadPage();

};
#endif
