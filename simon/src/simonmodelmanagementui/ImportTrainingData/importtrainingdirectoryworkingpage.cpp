/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "importtrainingdirectoryworkingpage.h"
#include <QUrl>
#include "importtrainingdata.h"
#include <KWidgetsAddons/KMessageBox>

/**
 * \brief Constructor - creates the GUI
 * \author Peter Grasch
 * @param parent
 * Parent of the widget
 */
ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget* parent) : QWizardPage(parent),
importer(new ImportTrainingData(this)),
completed(false)
{
  ui.setupUi(this);

  setTitle(i18n("Processing folder..."));

  connect(importer, SIGNAL(done()), this, SLOT(setComplete()));
  connect(importer, SIGNAL(progress(int,int)), this, SLOT(displayProgress(int,int)));
  connect(importer, SIGNAL(status(QString)), this, SLOT(displayStatus(QString)));
  connect(importer, SIGNAL(error(QString)), this, SLOT(displayError(QString)));
}


void ImportTrainingDirectoryWorkingPage::displayProgress(int now, int max)
{
  if (max != -1)
    ui.pbMain->setMaximum(max);
  ui.pbMain->setValue(now);
}


void ImportTrainingDirectoryWorkingPage::displayStatus(QString status)
{
  ui.lbStatus->setText(status);
}


void ImportTrainingDirectoryWorkingPage::displayError(QString error)
{
  KMessageBox::error(this, error);
}


void ImportTrainingDirectoryWorkingPage::setComplete()
{
  completed = true;
  emit completeChanged();

  emit done();
}

void ImportTrainingDirectoryWorkingPage::cleanupPage()
{
  if (importer->isRunning())
    importer->terminate();
}

/**
 * \brief Starts the importing process and calls all the other methods
 * \author Peter Grasch
 */
void ImportTrainingDirectoryWorkingPage::initializePage()
{
  completed = false;
  emit completeChanged();

  //reading
  bool importPrompts = field("importPrompts").toBool();
  QString path, basePath;
  if (importPrompts) {
    path = field("prompts").value<QUrl>().path();
    basePath = field("promptsBaseDirectory").value<QUrl>().path();
  }
  else {
    path = field("directory").value<QUrl>().path();
  }
  importer->import(importPrompts, path, basePath);
}

ImportTrainingDirectoryWorkingPage::~ImportTrainingDirectoryWorkingPage()
{
  importer->deleteLater();
}

