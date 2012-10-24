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

#include "importtrainingtexts.h"
#include <QWizardPage>
#include <QLabel>
#include <KListWidget>
#include <KLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QProgressBar>
#include <QRadioButton>
#include <KPushButton>
#include "importtrainingtextlocalpage.h"
#include "importtrainingtextaddpage.h"
#include "importtrainingtextworkingpage.h"
#include "importtrainingtextselectsourcepage.h"
#include <KStandardDirs>

/**
 * \brief Constructor
 * \author Peter Grasch
 */
ImportTrainingTexts::ImportTrainingTexts(QWidget* parent) : SimonWizard(parent),
prevId(0)
{
  this->addPage(createIntroPage());

  QWizardPage *source = createSourcePage();
  QWizardPage *add = createAddPage();
  QWizardPage *local = createLocalImportPage();
  QWizardPage *working = createWorkingPage();

  this->addPage(source);
  this->addPage(add);
  this->addPage(local);
  this->addPage(working);

  this->addPage(createFinishedPage());
  setWindowTitle(i18n("Add Trainingstext"));
  setBanner("importtexts");

}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportTrainingTexts::~ImportTrainingTexts()
{
}


/**
 * \brief Creates the intropage
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createIntroPage()
{
  QWizardPage *intro = new QWizardPage(this);
  intro->setTitle(i18n("Import a new Trainingstext"));
  QLabel *label = new QLabel(intro);
  label->setWordWrap(true);
  label->setText(i18n("Using this assistant you can import new Trainingstexts from the Internet or "
    "local files.\n\nThat way the training of simon will never get boring!"
    ));
  QVBoxLayout *layout = new QVBoxLayout(intro);
  layout->addWidget(label);
  intro->setLayout(layout);

  return intro;
}


/**
 * \brief Creates the localimportpage
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createLocalImportPage()
{
  ImportTrainingTextLocalPage *localImport = new ImportTrainingTextLocalPage(this);

  return localImport;
}


QWizardPage* ImportTrainingTexts::createAddPage()
{
  ImportTrainingTextAddPage *localAdd = new ImportTrainingTextAddPage(this);

  return localAdd;
}


/**
 * \brief Creates the sourcepage
 *
 * Here you can select where you want to import from (internet/file)
 *
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createSourcePage()
{
  ImportTrainingTextSelectSourcePage *source = new ImportTrainingTextSelectSourcePage(this);
  return source;
}


/**
 * \brief Creates the working page
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createWorkingPage()
{
  ImportTrainingTextWorkingPage *working= new ImportTrainingTextWorkingPage(this);
  return working;
}


/**
 * \brief Creates the finished-page
 * @return the QWizardPage
 */
QWizardPage* ImportTrainingTexts::createFinishedPage()
{
  QWizardPage *finished = new QWizardPage(this);
  finished->setTitle(i18n("Text added"));
  QLabel *label = new QLabel(finished);
  label->setWordWrap(true);
  label->setText(i18n("You have now added a new text.\n\nThank you for improving simon!"));
  QVBoxLayout *layout = new QVBoxLayout(finished);
  layout->addWidget(label);
  finished->setLayout(layout);

  return finished;
}
