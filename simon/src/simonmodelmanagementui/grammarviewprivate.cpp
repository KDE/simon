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

#include "grammarviewprivate.h"
#include "ImportGrammar/importgrammarwizard.h"
#include "MergeTerminals/mergeterminalswizard.h"
#include "RenameTerminal/renameterminalwizard.h"

#include <QSortFilterProxyModel>
#include <KInputDialog>

#include <KDebug>
#include <KMessageBox>
#include <KGlobal>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/grammar.h>

GrammarViewPrivate::GrammarViewPrivate(QWidget* parent): QWidget( parent)
{
  ui.setupUi(this);
  ui.pbImportTexts->setIcon(KIcon("document-import"));
  ui.pbRename->setIcon(KIcon("document-properties"));
  ui.pbMerge->setIcon(KIcon("arrow-down-double"));

  ui.pbAdd->setIcon(KIcon("list-add"));
  ui.pbDelete->setIcon(KIcon("list-remove"));

  connect(ui.lvStructures, SIGNAL(selectionChanged()), this, SLOT(currentSelectionChanged()));

  connect(ui.pbImportTexts, SIGNAL(clicked()), this, SLOT(showImportWizard()));
  connect(ui.pbMerge, SIGNAL(clicked()), this, SLOT(showMergeWizard()));

  connect(ui.pbAdd, SIGNAL(clicked()), this, SLOT(addStructure()));
  connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(deleteStructure()));

  connect (ui.pbRename, SIGNAL(clicked()), this, SLOT(showRenameWizard()));

  grammarProxy = new QSortFilterProxyModel();
  grammarProxy->setFilterKeyColumn(0);
  ui.lvStructures->setModel(grammarProxy);
  ui.lwExamples->clear();

  currentSelectionChanged();
}


void GrammarViewPrivate::currentSelectionChanged()
{
  ui.lwExamples->clear();

  int structureIndex = ui.lvStructures->currentIndex().row();

  if (structureIndex == -1) {
    ui.pbDelete->setEnabled(false);
  }
  else {
    ui.pbDelete->setEnabled(true);

    QString selectedStructure = ScenarioManager::getInstance()->getCurrentScenario()->grammar()->getStructure(structureIndex);

    QStringList examples = ScenarioManager::getInstance()->getCurrentScenario()->getExampleSentencesOfStructur(selectedStructure);
    ui.lwExamples->addItems(examples);
  }
}


void GrammarViewPrivate::addStructure()
{
  QString structure = KInputDialog::getText(i18n("Add structure"), i18n("Please enter the new grammar structure.\n\nNote: Use terminals instead of distinct words!"));
  if (structure.isEmpty()) return;

  if (!ScenarioManager::getInstance()->getCurrentScenario()->grammar()->addStructure(structure))
    KMessageBox::error(this, i18n("Could not add structure to the grammar."));
}


void GrammarViewPrivate::deleteStructure()
{
  int structureIndex = ui.lvStructures->currentIndex().row();
  if (structureIndex == -1) return;

  if (KMessageBox::questionYesNo(this, i18n("Do you really want to delete the selected grammar structure?"))==KMessageBox::Yes) {
    if (!ScenarioManager::getInstance()->getCurrentScenario()->grammar()->deleteStructure(structureIndex))
      KMessageBox::error(this, i18n("Could not delete structure."));
    else currentSelectionChanged();
  }
}


void GrammarViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
  kDebug() << "Displaying scenario " << scenario->name();

  Grammar *g = scenario->grammar();
  grammarProxy->setSourceModel(g);
  currentSelectionChanged();
}


void GrammarViewPrivate::showRenameWizard()
{
  QPointer<RenameTerminalWizard> renameTerminalWizard = new RenameTerminalWizard(this);
  renameTerminalWizard->restart();
  renameTerminalWizard->exec();
  delete renameTerminalWizard;
  currentSelectionChanged();
}


void GrammarViewPrivate::showImportWizard()
{
  QPointer<ImportGrammarWizard> importGrammarWizard = new ImportGrammarWizard(this);
  importGrammarWizard->exec();
  delete importGrammarWizard;
  currentSelectionChanged();
}


void GrammarViewPrivate::showMergeWizard()
{
  QPointer<MergeTerminalsWizard> mergeTerminalsWizard = new MergeTerminalsWizard(this);
  mergeTerminalsWizard->restart();
  mergeTerminalsWizard->exec();
  delete mergeTerminalsWizard;
  currentSelectionChanged();
}


GrammarViewPrivate::~GrammarViewPrivate()
{
}
