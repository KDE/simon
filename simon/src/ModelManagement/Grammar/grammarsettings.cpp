//
// C++ Implementation: grammarsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "grammarsettings.h"
#include "ImportGrammar/importgrammarwizard.h"
#include "grammarmanager.h"
#include "MergeTerminals/mergeterminalswizard.h"
#include "RenameTerminal/renameterminalwizard.h"
#include <QTableWidgetItem>
#include <kmessagebox.h>

GrammarSettings::GrammarSettings(QWidget* parent): SystemWidget(i18n("Grammatikeinstellungen"), KIcon("signature"), i18n("Grammatik bearbeiten"), parent)
{
	ui.setupUi(this);
	ui.pbImportTexts->setIcon(KIcon("document-open"));
	ui.pbRename->setIcon(KIcon("document-properties"));
	ui.pbMerge->setIcon(KIcon("arrow-down-double"));
	
	help = i18n("Hier können Sie die Grammatikkonstrukte die von simon erkannt werden anpassen.");

	this->importGrammarWizard = new ImportGrammarWizard(this);

	this->renameTerminalWizard = new RenameTerminalWizard(this);

	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	connect(importGrammarWizard, SIGNAL(finished(int)), ui.pbImportTexts, SLOT(toggle()));
	
	
	this->mergeTerminalsWizard = new MergeTerminalsWizard(this);
	
	connect(ui.pbImportTexts, SIGNAL(toggled(bool)), this, SLOT(showImportWizard(bool)));
	connect(ui.pbMerge, SIGNAL(toggled(bool)), this, SLOT(showMergeWizard(bool)));
	
	connect(mergeTerminalsWizard, SIGNAL(finished(int)), ui.pbMerge, SLOT(animateClick()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), ui.pbRename, SLOT(animateClick()));

	connect(mergeTerminalsWizard, SIGNAL(finished(int)), this, SLOT(reset()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), this, SLOT(reset()));

	connect(ui.elbSentences, SIGNAL(changed()), this, SIGNAL(changed()));
	
	connect (ui.pbRename, SIGNAL(toggled(bool)), this, SLOT(showRenameWizard(bool)));
}

void GrammarSettings::askForSave()
{
	if (KMessageBox::questionYesNo(this, i18n("Sie möchten eine Aktion ausführen, die eine gespeicherte Grammatik benötigt.\n\nWenn Sie Ihre aktuellen Änderungen beibehalten möchten, müssen Sie jetzt Ihre Grammatik speichern.(Ansonsten wird mit der zuletzt gespeicherten Grammatik weitergearbeitet)\n\nWollen Sie das jetzt tun?"), i18n("Grammatik speichern")) == KMessageBox::Yes)
		apply();
}

void GrammarSettings::showRenameWizard(bool show)
{
	if (show)
	{
		askForSave();
		this->renameTerminalWizard->restart();
		this->renameTerminalWizard->show();
	}else this->renameTerminalWizard->hide();
	
}


void GrammarSettings::mergeGrammar(QStringList grammar)
{
	QStringList toInsert;
	
	QStringList currentStructures = getCurrentStructures();
	for (int i=0; i < grammar.count(); i++)
	{
		if (!currentStructures.contains(grammar[i]))
			toInsert << grammar[i];
	}
	insertSentences(toInsert);
}

QStringList GrammarSettings::getCurrentTerminals()
{
	QStringList terms;
	QString sent;
	for (int i=0; i < ui.elbSentences->count(); i++)
	{
		sent = ui.elbSentences->text(i);
		QStringList termtemp = sent.split(" ");
		for (int j=0; j < termtemp.count(); j++)
		{
			if (!terms.contains(termtemp[j])) terms << termtemp[j];
		}
	}
	return terms;
}

void GrammarSettings::insertSentences(QStringList sentences)
{
	ui.elbSentences->insertStringList(sentences);
}

void GrammarSettings::showImportWizard(bool show)
{
	if (show) {
		importGrammarWizard->restart();
		importGrammarWizard->show();
	} else importGrammarWizard->hide();
}

void GrammarSettings::showMergeWizard(bool show)
{
	if (show) {
		askForSave();
		mergeTerminalsWizard->restart();
		mergeTerminalsWizard->show();
	} else mergeTerminalsWizard->hide();
}

/**
 * \brief Returns if true if we filled out the required fields
 * @return always true - there are no mandatory fields here
 */
bool GrammarSettings::isComplete()
{
	return true;
}


QStringList GrammarSettings::getCurrentStructures()
{
	return ui.elbSentences->items();
}

bool GrammarSettings::apply()
{
	GrammarManager *grammarManager = GrammarManager::getInstance();
	QStringList sentences = getCurrentStructures();
	grammarManager->setStructures(sentences);
	grammarManager->save();
	return true;
}

bool GrammarSettings::init()
{
	ui.elbSentences->clear();
	GrammarManager *grammarManager = GrammarManager::getInstance();
	QStringList terminals = grammarManager->getTerminals();

	QStringList sentences = grammarManager->getAllStructures();
	insertSentences(sentences);

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
