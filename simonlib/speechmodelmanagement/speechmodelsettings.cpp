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


#include "speechmodelsettings.h"
#include "ImportGrammar/importgrammarwizard.h"
#include "MergeTerminals/mergeterminalswizard.h"
#include "RenameTerminal/renameterminalwizard.h"
#include "speechmodelmanagementconfiguration.h"
#include <KMessageBox>
#include <KGlobal>
#include <KPageWidget>
#include "grammarmanager.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SpeechModelSettingsFactory, 
			registerPlugin< SpeechModelSettings >(); 
		)
        
K_EXPORT_PLUGIN( SpeechModelSettingsFactory("SpeechModelSettings") )

SpeechModelSettings::SpeechModelSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	KPageWidget *pageWidget = new KPageWidget(this);
	lay->addWidget(pageWidget);
	
	if (args.count() == 1)
		pageWidget->setFaceType(KPageView::Tabbed);
	
	QWidget *grammarWidget = new QWidget(this);
	uiGrammar.setupUi(grammarWidget);
	uiGrammar.pbImportTexts->setIcon(KIcon("document-open"));
	uiGrammar.pbRename->setIcon(KIcon("document-properties"));
	uiGrammar.pbMerge->setIcon(KIcon("arrow-down-double"));

	connect(uiGrammar.pbImportTexts, SIGNAL(clicked()), this, SLOT(showImportWizard()));
	connect (uiGrammar.pbRename, SIGNAL(clicked()), this, SLOT(showRenameWizard()));
	connect(uiGrammar.pbMerge, SIGNAL(clicked()), this, SLOT(showMergeWizard()));
	
	QWidget *trainingsDataWidget = new QWidget(this);
	uiTrainingsData.setupUi(trainingsDataWidget);
	
	KPageWidgetItem *grammar = pageWidget->addPage(grammarWidget, i18n("Grammatik"));
	KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Trainingsdaten"));

	grammar->setIcon(KIcon("user-properties"));
	trainingsData->setIcon(KIcon("view-pim-news"));

	grammar->setHeader("");
	trainingsData->setHeader("");
	
	addConfig(SpeechModelManagementConfiguration::self(), this);
}


void SpeechModelSettings::askToSaveGrammar()
{
	if (KMessageBox::questionYesNo(this, i18n("Sie möchten eine Aktion ausführen, die eine gespeicherte Grammatik benötigt.\n\nWenn Sie Ihre aktuellen Änderungen beibehalten möchten, müssen Sie jetzt Ihre Grammatik speichern.(Ansonsten wird mit der zuletzt gespeicherten Grammatik weitergearbeitet)\n\nWollen Sie das jetzt tun?"), i18n("Grammatik speichern")) == KMessageBox::Yes)
		save();
}


void SpeechModelSettings::mergeGrammar(QStringList grammar)
{
	QStringList toInsert;
	
	QStringList currentStructures = uiGrammar.kcfg_GrammarStructures->items();
	for (int i=0; i < grammar.count(); i++)
	{
		if (!currentStructures.contains(grammar[i]))
			toInsert << grammar[i];
	}
	uiGrammar.kcfg_GrammarStructures->insertStringList(toInsert);
}

void SpeechModelSettings::showImportWizard()
{
	ImportGrammarWizard *importGrammarWizard = new ImportGrammarWizard(this);
	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	connect(importGrammarWizard, SIGNAL(finished(int)), importGrammarWizard, SLOT(deleteLater()));
	importGrammarWizard->show();
}

void SpeechModelSettings::showRenameWizard()
{
	askToSaveGrammar();

	RenameTerminalWizard *renameTerminalWizard = new RenameTerminalWizard(this);
	connect(renameTerminalWizard, SIGNAL(finished(int)), this, SLOT(load()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), renameTerminalWizard, SLOT(deleteLater()));
	renameTerminalWizard->show();
}

void SpeechModelSettings::showMergeWizard()
{
	askToSaveGrammar();

	MergeTerminalsWizard *mergeTerminalsWizard = new MergeTerminalsWizard(this);
	connect(mergeTerminalsWizard, SIGNAL(finished(int)), this, SLOT(reset()));
	connect(mergeTerminalsWizard, SIGNAL(finished(int)), mergeTerminalsWizard, SLOT(deleteLater()));
}


SpeechModelSettings::~SpeechModelSettings()
{
}
