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
#include <QInputDialog>

#include <KDebug>
#include <KMessageBox>
#include <KGlobal>
#include <speechmodelmanagement/scenariomanager.h>
#include <speechmodelmanagement/scenario.h>
#include <speechmodelmanagement/grammar.h>


GrammarViewPrivate::GrammarViewPrivate(QWidget* parent): QWidget( parent)
{
	ui.setupUi(this);
	ui.pbImportTexts->setIcon(KIcon("document-open"));
	ui.pbRename->setIcon(KIcon("document-properties"));
	ui.pbMerge->setIcon(KIcon("arrow-down-double"));

	ui.pbAdd->setIcon(KIcon("list-add"));
	ui.pbDelete->setIcon(KIcon("list-remove"));

	//connect(&autoSaveTimer, SIGNAL(timeout()), this, SLOT(save()));
	
//	connect(ui.lvStructures, SIGNAL(activated(const QModelIndex&)), this, SLOT(currentSelectionChanged()));
	connect(ui.lvStructures, SIGNAL(clicked(const QModelIndex&)), this, SLOT(currentSelectionChanged()));
	
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
	} else {
		ui.pbDelete->setEnabled(true);

		QString selectedStructure = ScenarioManager::getInstance()->getCurrentScenario()->grammar()->getStructure(structureIndex);

		QStringList examples = ScenarioManager::getInstance()->getCurrentScenario()->getAllPossibleSentencesOfStructure(selectedStructure);
		ui.lwExamples->addItems(examples);
	}
}

void GrammarViewPrivate::addStructure()
{
	QString structure = QInputDialog::getText(this, i18n("Add structure"), i18n("Please enter the new grammar structure.\n\nNote: Use terminals instead of distinct words!"));
	if (structure.isEmpty()) return;

	if (!ScenarioManager::getInstance()->getCurrentScenario()->grammar()->addStructure(structure))
		KMessageBox::error(this, i18n("Couldn't add structure to the grammar."));
}

void GrammarViewPrivate::deleteStructure()
{
	int structureIndex = ui.lvStructures->currentIndex().row();
	if (structureIndex == -1) return;

	if (KMessageBox::questionYesNo(this, i18n("Do you really want to delete the selected grammar structure?"))==KMessageBox::Yes) {
		if (!ScenarioManager::getInstance()->getCurrentScenario()->grammar()->deleteStructure(structureIndex))
			KMessageBox::error(this, i18n("Couldn't delete structure."));
		else currentSelectionChanged();
	}
}


void GrammarViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
	kDebug() << "Displaying scenario " << scenario->name();

	Grammar *g = scenario->grammar();
	grammarProxy->setSourceModel(g);
}

//void GrammarViewPrivate::slotChanged()
//{
//	autoSaveTimer.start(5000);
//}

void GrammarViewPrivate::showRenameWizard()
{
	RenameTerminalWizard *renameTerminalWizard = new RenameTerminalWizard(this);
	renameTerminalWizard->restart();
	renameTerminalWizard->exec();
	renameTerminalWizard->deleteLater();
}


void GrammarViewPrivate::showImportWizard()
{
	ImportGrammarWizard *importGrammarWizard = new ImportGrammarWizard(this);
	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	importGrammarWizard->exec();
	importGrammarWizard->deleteLater();
}

void GrammarViewPrivate::showMergeWizard()
{
	MergeTerminalsWizard *mergeTerminalsWizard = new MergeTerminalsWizard(this);
	mergeTerminalsWizard->restart();
	mergeTerminalsWizard->exec();
	mergeTerminalsWizard->deleteLater();
}


GrammarViewPrivate::~GrammarViewPrivate()
{
}
