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

#include "importtrainingdirectory.h"
#include "importtrainingdirectoryintropage.h"
#include "importtrainingdirectoryworkingpage.h"

#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KLineEdit>
#include <QDir>
#include <KPushButton>
#include <QIcon>
#include <QCoreApplication>
#include <KStandardDirs>

/**
 * \brief Constructor - Creats the pages and adds them
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectory::ImportTrainingDirectory(QWidget* parent) : SimonWizard(parent),
prevId(0)
{
  setWindowTitle(i18n("Importing Training Samples"));
  setBanner("importfolder");

  addPage(createIntroPage());
  addPage(createWorkingPage());
  addPage(createFinishedPage());
}


/**
 * \brief Creates a new ImoprtTrainingDirectoryIntroPage
 * @return the wizard page
 */
ImportTrainingDirectoryIntroPage* ImportTrainingDirectory::createIntroPage()
{
  ImportTrainingDirectoryIntroPage *introPage =  new
    ImportTrainingDirectoryIntroPage(this);
  return introPage;
}


/**
 * \brief creates the working-page
 * @return the newly created ImportTrainingDirectoryWorkingPage
 */
ImportTrainingDirectoryWorkingPage* ImportTrainingDirectory::createWorkingPage()
{

  ImportTrainingDirectoryWorkingPage *page = new ImportTrainingDirectoryWorkingPage(this);
  connect(page, SIGNAL(done()), this, SLOT(next()));
  return  page;
}


/**
 * \brief Creates the Finished page
 * @return the page
 */
QWizardPage* ImportTrainingDirectory::createFinishedPage()
{
  QWizardPage *intro = new QWizardPage(this);
  intro->setTitle(i18n("Importing of the Folder completed"));
  QLabel *label = new QLabel(intro);
  label->setText(i18n("The files have been imported.\n\nThank you for "
    "improving Simon!"));
  QVBoxLayout *layout = new QVBoxLayout(intro);
  layout->addWidget(label);
  intro->setLayout(layout);

  return intro;
}
