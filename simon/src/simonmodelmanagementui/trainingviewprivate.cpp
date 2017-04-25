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

#include "trainingviewprivate.h"

#include <simonscenarios/trainingmanager.h>
#include <simonscenarios/trainingtextcollection.h>
#include <simonscenarios/scenario.h>
#include "ImportTrainingTexts/importtrainingtexts.h"
#include "TrainSamples/trainingswizard.h"
#include "promptsview.h"

#include <simonsound/recwidget.h>
#include <simoninfo/simoninfo.h>

#include <QHash>
#include <QHashIterator>
#include <QWidget>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <KMessageBox>

/**
 * \brief Constructor - inits the Gui
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
TrainingViewPrivate::TrainingViewPrivate ( QWidget *parent )
: QWidget(parent)
{
  ui.setupUi ( this );

  connect(ui.pbTrainText, SIGNAL (clicked()), this, SLOT (trainSelected()));
  connect(ui.tvTrainingTexts, SIGNAL (doubleClicked(QModelIndex)), this, SLOT (trainSelected()));

  connect(ui.pbImportText, SIGNAL (clicked()), this, SLOT (importTexts()));
  connect(ui.pbDelText, SIGNAL (clicked()), this, SLOT (deleteSelected()));
  connect(ui.pbManageSamples, SIGNAL (clicked()), this, SLOT (manageSamples()));

  textsProxy = new QSortFilterProxyModel();
  textsProxy->setFilterKeyColumn(0);
  ui.tvTrainingTexts->setModel(textsProxy);

  //set up icons
  ui.pbTrainText->setIcon(QIcon::fromTheme("go-next"));
  ui.pbDelText->setIcon(QIcon::fromTheme("edit-delete"));
  ui.pbImportText->setIcon(QIcon::fromTheme("document-import"));
  ui.pbManageSamples->setIcon(QIcon::fromTheme("view-list-tree"));
}


/**
 * \brief Deletes the selected text from the harddisc
 *
 * Asks the user for confirmation before the irreversible deletion
 * \author Peter Grasch
 */
void TrainingViewPrivate::deleteSelected()
{
  TrainingText* text = getCurrentlySelectedText();
  if (!text)
    return;

  if (KMessageBox::questionYesNo(this, i18n("Do you really want to delete the selected text?"))== KMessageBox::Yes)
    if (!ScenarioManager::getInstance()->getCurrentScenario()->removeText(text))
      KMessageBox::sorry(this, i18n("Failed to delete text.\n\nMaybe you do not have the necessary permissions?"));
}


/**
 * \brief Starts the training of the selected text
 * \author Peter Grasch
 */
void TrainingViewPrivate::trainSelected()
{
  TrainingText* text = getCurrentlySelectedText();
  if (!text)
    return;

  QPointer<TrainingsWizard> wizard = new TrainingsWizard(this);

  if (wizard->init(*text))
    wizard->exec();

  delete wizard;
}


TrainingText* TrainingViewPrivate::getCurrentlySelectedText()
{
  QModelIndex selectedIndex = textsProxy->mapToSource(ui.tvTrainingTexts->currentIndex());
  if (!selectedIndex.isValid()) {
    KMessageBox::information(this, i18n("Please select a text first"));
    return 0;
  }

  return static_cast<TrainingText*>(selectedIndex.internalPointer());
}

void TrainingViewPrivate::importTexts()
{
  QPointer<ImportTrainingTexts> import(new ImportTrainingTexts(this));
  import->exec();
  delete import;
}


void TrainingViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
  TrainingTextCollection *t = scenario->texts();
  textsProxy->setSourceModel(t);
}

void TrainingViewPrivate::manageSamples()
{
  QPointer<PromptsView> p(new PromptsView(this));
  p->exec();
  delete p;
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingViewPrivate::~TrainingViewPrivate()
{
}
