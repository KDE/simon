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
#include "grammarmanager.h"
#include "MergeTerminals/mergeterminalswizard.h"
#include "RenameTerminal/renameterminalwizard.h"
#include <kmessagebox.h>
#include "coreconfiguration.h"

GrammarSettings::GrammarSettings(QWidget* parent): QWidget(parent) //SystemWidget(i18n("Grammatikeinstellungen"), KIcon("user-properties"), i18n("Grammatik bearbeiten"), parent)
{
	ui.setupUi(this);
	ui.pbImportTexts->setIcon(KIcon("document-open"));
	ui.pbRename->setIcon(KIcon("document-properties"));
	ui.pbMerge->setIcon(KIcon("arrow-down-double"));
	
// 	help = i18n("Hier können Sie die Grammatikkonstrukte die von simon erkannt werden anpassen.");

	this->importGrammarWizard = new ImportGrammarWizard(this);

	this->renameTerminalWizard = new RenameTerminalWizard(this);

	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	connect(importGrammarWizard, SIGNAL(finished(int)), ui.pbImportTexts, SLOT(toggle()));
	
	
	this->mergeTerminalsWizard = new MergeTerminalsWizard(this);
	
	connect(ui.pbImportTexts, SIGNAL(clicked()), this, SLOT(showImportWizard()));
	connect(ui.pbMerge, SIGNAL(clicked()), this, SLOT(showMergeWizard()));

	connect(mergeTerminalsWizard, SIGNAL(finished(int)), this, SLOT(reset()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), this, SLOT(reset()));

	connect(ui.kcfg_GrammarStructures, SIGNAL(changed()), this, SIGNAL(changed()));
	
	connect (ui.pbRename, SIGNAL(clicked()), this, SLOT(showRenameWizard()));
}

void GrammarSettings::askForSave()
{
	if (KMessageBox::questionYesNo(this, i18n("Sie möchten eine Aktion ausführen, die eine gespeicherte Grammatik benötigt.\n\nWenn Sie Ihre aktuellen Änderungen beibehalten möchten, müssen Sie jetzt Ihre Grammatik speichern.(Ansonsten wird mit der zuletzt gespeicherten Grammatik weitergearbeitet)\n\nWollen Sie das jetzt tun?"), i18n("Grammatik speichern")) == KMessageBox::Yes)
		apply();
}

void GrammarSettings::showRenameWizard()
{
	askForSave();
	this->renameTerminalWizard->restart();
	this->renameTerminalWizard->show();
	
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

// QStringList GrammarSettings::getCurrentTerminals()
// {
// 	QStringList terms;
// 	QString sent;
// 	for (int i=0; i < ui.kcfg_GrammarStructures->count(); i++)
// 	{
// 		sent = ui.kcfg_GrammarStructures->text(i);
// 		QStringList termtemp = sent.split(" ");
// 		for (int j=0; j < termtemp.count(); j++)
// 		{
// 			if (!terms.contains(termtemp[j])) terms << termtemp[j];
// 		}
// 	}
// 	return terms;
// }

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

/**
 * \brief Returns if true if we filled out the required fields
 * @return always true - there are no mandatory fields here
 */
// bool GrammarSettings::isComplete()
// {
// 	return true;
// }


// QStringList GrammarSettings::getCurrentStructures()
// {
// 	return ui.kcfg_GrammarStructures->items();
// }

bool GrammarSettings::apply()
{
// 	GrammarManager *grammarManager = GrammarManager::getInstance();
// 	QStringList sentences = ui.kcfg_GrammarStructures->items();
// 	grammarManager->setStructures(sentences);
// 	grammarManager->save();
	CoreConfiguration::setGrammarStructures(ui.kcfg_GrammarStructures->items());
	CoreConfiguration::self()->writeConfig();
	return true;
}

bool GrammarSettings::init()
{
// 	ui.kcfg_GrammarStructures->clear();
// 	GrammarManager *grammarManager = GrammarManager::getInstance();
// 	QStringList sentences = grammarManager->getAllStructures();
// 	insertSentences(sentences);

	return true;
}

bool GrammarSettings::reset()
{
	return init();
}


GrammarSettings::~GrammarSettings()
{
    importGrammarWizard->deleteLater();
    renameTerminalWizard->deleteLater();
    mergeTerminalsWizard->deleteLater();
}
