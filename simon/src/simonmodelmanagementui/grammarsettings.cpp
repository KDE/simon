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


#include "grammarsettings.h"
#include "ImportGrammar/importgrammarwizard.h"
#include "MergeTerminals/mergeterminalswizard.h"
#include "RenameTerminal/renameterminalwizard.h"

#include <KMessageBox>
#include <KGlobal>
#include "grammarmanager.h"


GrammarSettings::GrammarSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
	Q_UNUSED(args);

	
	ui.setupUi(this);
	ui.pbImportTexts->setIcon(KIcon("document-open"));
	ui.pbRename->setIcon(KIcon("document-properties"));
	ui.pbMerge->setIcon(KIcon("arrow-down-double"));


	connect(GrammarManager::getInstance(), SIGNAL(structuresChanged()), this, SLOT(load()));

	this->importGrammarWizard = new ImportGrammarWizard(this);

	this->renameTerminalWizard = new RenameTerminalWizard(this);

	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	connect(importGrammarWizard, SIGNAL(finished(int)), ui.pbImportTexts, SLOT(toggle()));
	
	
	this->mergeTerminalsWizard = new MergeTerminalsWizard(this);
	
	connect(ui.pbImportTexts, SIGNAL(clicked()), this, SLOT(showImportWizard()));
	connect(ui.pbMerge, SIGNAL(clicked()), this, SLOT(showMergeWizard()));

	connect(mergeTerminalsWizard, SIGNAL(finished(int)), this, SLOT(load()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), this, SLOT(load()));

	connect(ui.kcfg_GrammarStructures, SIGNAL(changed()), this, SLOT(slotChanged()));
	
	connect (ui.pbRename, SIGNAL(clicked()), this, SLOT(showRenameWizard()));
}

void GrammarSettings::askForSave()
{
	if (KMessageBox::questionYesNo(this, i18n("You are about to starts a process that needs the grammar to be saved first.\n\nIf you want to keep the changes that you possibly made since the last time you stored your grammar, please save the grammar now.\n\nDo you want to save your grammar now?"), i18n("Save Grammar")) == KMessageBox::Yes)
		save();
}

void GrammarSettings::slotChanged()
{
	emit changed(true);
}

void GrammarSettings::showRenameWizard()
{
	askForSave();
	this->renameTerminalWizard->restart();
	this->renameTerminalWizard->show();
}



void GrammarSettings::load()
{
	ui.kcfg_GrammarStructures->setItems(GrammarManager::getInstance()->getStructures());
	emit changed(false);
}

void GrammarSettings::save()
{
	GrammarManager::getInstance()->setStructures(ui.kcfg_GrammarStructures->items());
	GrammarManager::getInstance()->save();
	emit changed(false);
}

void GrammarSettings::defaults()
{
	ui.kcfg_GrammarStructures->clear();
	save();
}


void GrammarSettings::mergeGrammar(QStringList grammar)
{
	QStringList toInsert;
	
	QStringList currentStructures = ui.kcfg_GrammarStructures->items();
	for (int i=0; i < grammar.count(); i++)
	{
		if (!currentStructures.contains(grammar[i]))
			toInsert << grammar[i];
	}
	ui.kcfg_GrammarStructures->insertStringList(toInsert);
}


void GrammarSettings::showImportWizard()
{
	importGrammarWizard->restart();
	importGrammarWizard->show();
}

void GrammarSettings::showMergeWizard()
{
	askForSave();
	mergeTerminalsWizard->restart();
	mergeTerminalsWizard->show();
}


GrammarSettings::~GrammarSettings()
{
    importGrammarWizard->deleteLater();
    renameTerminalWizard->deleteLater();
    mergeTerminalsWizard->deleteLater();
}
