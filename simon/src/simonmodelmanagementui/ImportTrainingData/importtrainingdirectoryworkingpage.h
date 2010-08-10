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

#ifndef SIMON_IMPORTTRAININGDIRECTORYWORKINGPAGE_H_5EDEFF0801CB414EB61CF8760BC11C86
#define SIMON_IMPORTTRAININGDIRECTORYWORKINGPAGE_H_5EDEFF0801CB414EB61CF8760BC11C86

#include <QWizardPage>
#include "ui_importtrainingdataworkingdlg.h"

class ImportTrainingData;

/**
 * \class ImportTrainingDirectoryWorkingPage
 * \author Peter Grasch
 * \version 0.1
 * \brief Imports the given directory
 *
 * Starts the import process per importDir():
 *   * Create Prompts
 *   * Resample WAVs to a new dest.dir
 *   * Normalize WAVs
 *   * Write codetrain.scp
 *
 * Normalization and resampling is done with external programs
 * (resample and normalize-audio)
 *
 * Extends QWizardpage;
 */
class ImportTrainingDirectoryWorkingPage : public QWizardPage
{
  Q_OBJECT

  signals:
    void done();

  private:
    Ui::ImportTrainingDataWorkingDlg ui;

    ImportTrainingData *importer;
    bool completed;

    void error() { completed = false; }

  private slots:
    void setComplete();
    void displayProgress(int now, int max);
    void displayStatus(QString status);
    void displayError(QString error);

  public:
    ImportTrainingDirectoryWorkingPage(QWidget *parent=0);
    ~ImportTrainingDirectoryWorkingPage();
    // 	bool importDir(QString dir);
    bool isComplete() const { return completed; }
    void initializePage();
    void cleanupPage();

};
#endif
