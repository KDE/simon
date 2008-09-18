//
// C++ Implementation: modellsettings
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "modelsettings.h"
#include "../SimonLib/Settings/settings.h"
#include <QTableWidget>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <KLineEdit>

ModelSettings::ModelSettings(QWidget* parent): SystemWidget(i18n("Modelleinstellungen"), QIcon(":/images/icons/applications-education-language.svg"), i18n("Hier können Sie Einstellungen rund um das Sprachmodell einstellen"), parent)
{
	ui.setupUi(this);
	connect(ui.pbAdd, SIGNAL(clicked()), this, SLOT(addFilter()));
	connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteFilter()));
	connect(ui.tbUp, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(ui.tbDown, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(ui.twProcessingFilters, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(enableButtons()));
	
	connect(ui.urLexicon, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urGrammar, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urPrompts, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urVocab, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urWavConfig, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urConfig, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urProto, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urTreeHed, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urMkPhones0, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urMkPhones1, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urSamplePath, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urGlobalDed, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urSilHed, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urMktriLed, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urHmmOut, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urTiedlist, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urShadowLexicon, SIGNAL(urlChanged()), this, SIGNAL(changed()));
	connect(ui.urShadowVocab, SIGNAL(urlChanged()), this, SIGNAL(changed()));

	help = i18n("Hier können Sie Pfade zu den Sprachmodell-Dateien festlegen und Nachbearbeitungs-Ketten konfigurieren.\n\nAchtung: Unbedachte Änderungen in diesem Modul können die Funktionsfähigkeit von simon negativ beeinflussen");
}


ModelSettings::~ModelSettings()
{}



void ModelSettings::deleteFilter()
{
	if (ui.twProcessingFilters->currentRow()==-1)
	{
		return;
	}

	ui.twProcessingFilters->removeRow(ui.twProcessingFilters->currentRow());
	enableButtons();
}

void ModelSettings::addFilter()
{
	QString command="";
	bool ok=false;
	command=QInputDialog::getText ( this, i18n("Neues Kommando hinzufügen"),i18n("Vorverarbeitungskommando hinzufügen:"), KLineEdit::Normal,"", &ok );
	if ( ok && !command.isEmpty() )
	{
		insertFilter(command);
	}
}

void ModelSettings::insertFilter(QString command)
{
	int currRow = ui.twProcessingFilters->currentRow();
	if (currRow == -1) currRow = 0;

	ui.twProcessingFilters->insertRow(currRow);
	
	ui.twProcessingFilters->setItem(currRow,0, new QTableWidgetItem(command));
	enableButtons();
}

void ModelSettings::enableButtons()
{
	if (ui.twProcessingFilters->currentRow() == -1)
	{
		ui.pbRemove->setEnabled(false);
		ui.tbDown->setEnabled(false);
		ui.tbUp->setEnabled(false);
		return;
	} else ui.pbRemove->setEnabled(true);

	
	if (ui.twProcessingFilters->rowCount()-1 > ui.twProcessingFilters->currentRow())
		ui.tbDown->setEnabled(true);
	else ui.tbDown->setEnabled(false);
	
	if (0 < ui.twProcessingFilters->currentRow())
		ui.tbUp->setEnabled(true);
	else ui.tbUp->setEnabled(false);
}

void ModelSettings::moveUp()
{
	int currentRow = ui.twProcessingFilters->currentRow();
	if (!ui.twProcessingFilters->item(currentRow,0)) return;
	if (!ui.twProcessingFilters->item(currentRow-1,0)) return;
	
	QString command = ui.twProcessingFilters->item(currentRow,0)->text();
	
	ui.twProcessingFilters->item(currentRow,0)->setText(
		ui.twProcessingFilters->item(currentRow-1,0)->text());

	
	ui.twProcessingFilters->item(currentRow-1,0)->setText(command);
	ui.twProcessingFilters->setCurrentCell(currentRow-1, 0);

	enableButtons();
}

void ModelSettings::moveDown()
{
	int currentRow = ui.twProcessingFilters->currentRow();
	if (!ui.twProcessingFilters->item(currentRow,0)) return;
	if (!ui.twProcessingFilters->item(currentRow+1,0)) return;
	
	QString command = ui.twProcessingFilters->item(currentRow,0)->text();
	
	ui.twProcessingFilters->item(currentRow,0)->setText(
		ui.twProcessingFilters->item(currentRow+1,0)->text());

	
	ui.twProcessingFilters->item(currentRow+1,0)->setText(command);
	ui.twProcessingFilters->setCurrentCell(currentRow+1, 0);
	
	enableButtons();
}


bool ModelSettings::isComplete()
{
	return (!ui.urLexicon->url().isEmpty() && 
			!ui.urGrammar->url().isEmpty() && 
			!ui.urPrompts->url().isEmpty() && 
			!ui.urVocab->url().isEmpty() && 
			!ui.urWavConfig->url().isEmpty() && 
			!ui.urConfig->url().isEmpty() && 
			!ui.urProto->url().isEmpty() && 
			!ui.urTreeHed->url().isEmpty() && 
			!ui.urMkPhones0->url().isEmpty() && 
			!ui.urMkPhones1->url().isEmpty() && 
			!ui.urSamplePath->url().isEmpty() && 
			!ui.urGlobalDed->url().isEmpty() && 
			!ui.urSilHed->url().isEmpty() && 
			!ui.urMktriLed->url().isEmpty() && 
			!ui.urHmmOut->url().isEmpty() && 
			!ui.urTiedlist->url().isEmpty() && 
			!ui.urShadowLexicon->url().isEmpty() && 
			!ui.urDict->url().isEmpty() && 
			!ui.urDfa->url().isEmpty() && 
			!ui.urShadowVocab->url().isEmpty());
}


bool ModelSettings::apply()
{
	Settings::set("Model/Samplerate", ui.sbSamplerate->value());
	Settings::set("Model/Channels", ui.sbChannels->value());
	

	Settings::set("Model/PathToLexicon", ui.urLexicon->url().pathOrUrl());
	Settings::set("Model/PathToGrammar", ui.urGrammar->url().pathOrUrl());
	Settings::set("Model/PathToPrompts", ui.urPrompts->url().pathOrUrl());
	Settings::set("Model/PathToVocab", ui.urVocab->url().pathOrUrl());
	Settings::set("Model/PathToWavConfig", ui.urWavConfig->url().pathOrUrl());
	Settings::set("Model/PathToConfig", ui.urConfig->url().pathOrUrl());
	Settings::set("Model/PathToProto", ui.urProto->url().pathOrUrl());
	Settings::set("Model/PathToTreeHed", ui.urTreeHed->url().pathOrUrl());

	Settings::set("Model/PathToMkPhones0", ui.urMkPhones0->url().pathOrUrl());
	Settings::set("Model/PathToMkPhones1", ui.urMkPhones1->url().pathOrUrl());

	Settings::set("Model/PathToSamples", ui.urSamplePath->url().pathOrUrl());
	Settings::set("Model/PathToGlobalDed", ui.urGlobalDed->url().pathOrUrl());
	Settings::set("Model/PathToSilHed", ui.urSilHed->url().pathOrUrl());
	Settings::set("Model/PathToMktriLed", ui.urMktriLed->url().pathOrUrl());
	Settings::set("Model/PathToPhonemes", ui.urMonophones->url().pathOrUrl());

	Settings::set("Model/PathToHmm", ui.urHmmOut->url().pathOrUrl());
	Settings::set("Model/PathToTiedlist", ui.urTiedlist->url().pathOrUrl());


	Settings::set("Model/PathToShadowLexicon", ui.urShadowLexicon->url().pathOrUrl());
	Settings::set("Model/PathToShadowVocab", ui.urShadowVocab->url().pathOrUrl());


	Settings::set("Model/PathToDict", ui.urDict->url().pathOrUrl());
	Settings::set("Model/PathToDfa", ui.urDfa->url().pathOrUrl());


	Settings::set("Model/ProcessInternal", ui.cbProcessInternal->isChecked());

	if (ui.twProcessingFilters->rowCount() == 0) return true;
	
	if (ui.twProcessingFilters->rowCount() == 1)
		Settings::set("Model/ProcessingFilters", ui.twProcessingFilters->item(0,0)->text());

	if (ui.twProcessingFilters->rowCount() > 1)
	{
		QString filters;
		for (int i=0; i < ui.twProcessingFilters->rowCount()-1; i++)
		{
			filters += ui.twProcessingFilters->item(i,0)->text() + " && ";
		}
		filters += ui.twProcessingFilters->item(ui.twProcessingFilters->rowCount()-1,0)->text();
		Settings::set("Model/ProcessingFilters", filters);
	}

	return true;
}



bool ModelSettings::init()
{
	ui.sbSamplerate->setValue(Settings::getI("Model/Samplerate"));
	ui.sbChannels->setValue(Settings::getI("Model/Channels"));

	ui.cbProcessInternal->setChecked(Settings::getB("Model/ProcessInternal"));

	QStringList filters = Settings::getS("Model/ProcessingFilters").split("&&", QString::SkipEmptyParts);
	
	ui.twProcessingFilters->setRowCount(filters.count());
	for (int i=0; i < filters.count(); i++)
	{
		ui.twProcessingFilters->setItem(i, 0, new QTableWidgetItem(filters.at(i)));
	}
	ui.twProcessingFilters->resizeColumnToContents(0);
	enableButtons();

	ui.urLexicon->setPath(Settings::getS("Model/PathToLexicon"));
	ui.urGrammar->setPath(Settings::getS("Model/PathToGrammar"));
	ui.urPrompts->setPath(Settings::getS("Model/PathToPrompts"));
	ui.urVocab->setPath(Settings::getS("Model/PathToVocab"));
	ui.urWavConfig->setPath(Settings::getS("Model/PathToWavConfig"));

	ui.urConfig->setPath(Settings::getS("Model/PathToConfig"));
	ui.urProto->setPath(Settings::getS("Model/PathToProto"));
	ui.urTreeHed->setPath(Settings::getS("Model/PathToTreeHed"));


	ui.urMkPhones0->setPath(Settings::getS("Model/PathToMkPhones0"));
	ui.urMkPhones1->setPath(Settings::getS("Model/PathToMkPhones1"));

	ui.urSamplePath->setPath(Settings::getS("Model/PathToSamples"));
	ui.urGlobalDed->setPath(Settings::getS("Model/PathToGlobalDed"));
	ui.urSilHed->setPath(Settings::getS("Model/PathToSilHed"));
	ui.urMktriLed->setPath(Settings::getS("Model/PathToMktriLed"));

	ui.urHmmOut->setPath(Settings::getS("Model/PathToHmm"));
	ui.urTiedlist->setPath(Settings::getS("Model/PathToTiedlist"));

	ui.urShadowLexicon->setPath(Settings::getS("Model/PathToShadowLexicon"));
	ui.urShadowVocab->setPath(Settings::getS("Model/PathToShadowVocab"));

	ui.urDict->setPath(Settings::getS("Model/PathToDict"));
	ui.urDfa->setPath(Settings::getS("Model/PathToDfa"));

	ui.urMonophones->setPath(Settings::getS("Model/PathToPhonemes"));
	return true;
}

bool ModelSettings::reset()
{
	return init();
}

