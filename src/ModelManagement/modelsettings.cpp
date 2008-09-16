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

ModelSettings::ModelSettings(QWidget* parent): SystemWidget(tr("Modelleinstellungen"), QIcon(":/images/icons/applications-education-language.svg"), tr("Hier können Sie Einstellungen rund um das Sprachmodell einstellen"), parent)
{
	ui.setupUi(this);
	connect(ui.pbAdd, SIGNAL(clicked()), this, SLOT(addFilter()));
	connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteFilter()));
	connect(ui.tbUp, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(ui.tbDown, SIGNAL(clicked()), this, SLOT(moveDown()));
	connect(ui.twProcessingFilters, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(enableButtons()));
	
	connect(ui.leLexicon, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leGrammar, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.lePrompts, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leVocab, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leWavConfig, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leConfig, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leProto, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leTreeHed, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leMkPhones0, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leMkPhones1, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leSamplePath, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leGlobalDed, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leSilHed, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leMktriLed, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leHmmOut, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leTiedlist, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leShadowLexicon, SIGNAL(editingFinished()), this, SIGNAL(changed()));
	connect(ui.leShadowVocab, SIGNAL(editingFinished()), this, SIGNAL(changed()));

	help = tr("Hier können Sie Pfade zu den Sprachmodell-Dateien festlegen und Nachbearbeitungs-Ketten konfigurieren.\n\nAchtung: Unbedachte Änderungen in diesem Modul können die Funktionsfähigkeit von simon negativ beeinflussen");
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
	command=QInputDialog::getText ( this, tr("Neues Kommando hinzufügen"),tr("Vorverarbeitungskommando hinzufügen:"), KLineEdit::Normal,"", &ok );
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
	return (!ui.leLexicon->text().isEmpty() && 
			!ui.leGrammar->text().isEmpty() && 
			!ui.lePrompts->text().isEmpty() && 
			!ui.leVocab->text().isEmpty() && 
			!ui.leWavConfig->text().isEmpty() && 
			!ui.leConfig->text().isEmpty() && 
			!ui.leProto->text().isEmpty() && 
			!ui.leTreeHed->text().isEmpty() && 
			!ui.leMkPhones0->text().isEmpty() && 
			!ui.leMkPhones1->text().isEmpty() && 
			!ui.leSamplePath->text().isEmpty() && 
			!ui.leGlobalDed->text().isEmpty() && 
			!ui.leSilHed->text().isEmpty() && 
			!ui.leMktriLed->text().isEmpty() && 
			!ui.leHmmOut->text().isEmpty() && 
			!ui.leTiedlist->text().isEmpty() && 
			!ui.leShadowLexicon->text().isEmpty() && 
			!ui.leDict->text().isEmpty() && 
			!ui.leDfa->text().isEmpty() && 
			!ui.leShadowVocab->text().isEmpty());
}


bool ModelSettings::apply()
{
	Settings::set("Model/Samplerate", ui.sbSamplerate->value());
	Settings::set("Model/Channels", ui.sbChannels->value());
	

	Settings::set("Model/PathToLexicon", ui.leLexicon->text());
	Settings::set("Model/PathToGrammar", ui.leGrammar->text());
	Settings::set("Model/PathToPrompts", ui.lePrompts->text());
	Settings::set("Model/PathToVocab", ui.leVocab->text());
	Settings::set("Model/PathToWavConfig", ui.leWavConfig->text());
	Settings::set("Model/PathToConfig", ui.leConfig->text());
	Settings::set("Model/PathToProto", ui.leProto->text());
	Settings::set("Model/PathToTreeHed", ui.leTreeHed->text());

	Settings::set("Model/PathToMkPhones0", ui.leMkPhones0->text());
	Settings::set("Model/PathToMkPhones1", ui.leMkPhones1->text());

	Settings::set("Model/PathToSamples", ui.leSamplePath->text());
	Settings::set("Model/PathToGlobalDed", ui.leGlobalDed->text());
	Settings::set("Model/PathToSilHed", ui.leSilHed->text());
	Settings::set("Model/PathToMktriLed", ui.leMktriLed->text());
	Settings::set("Model/PathToPhonemes", ui.leMonophones->text());

	Settings::set("Model/PathToHmm", ui.leHmmOut->text());
	Settings::set("Model/PathToTiedlist", ui.leTiedlist->text());


	Settings::set("Model/PathToShadowLexicon", ui.leShadowLexicon->text());
	Settings::set("Model/PathToShadowVocab", ui.leShadowVocab->text());


	Settings::set("Model/PathToDict", ui.leDict->text());
	Settings::set("Model/PathToDfa", ui.leDfa->text());


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

	ui.leLexicon->setText(Settings::getS("Model/PathToLexicon"));
	ui.leGrammar->setText(Settings::getS("Model/PathToGrammar"));
	ui.lePrompts->setText(Settings::getS("Model/PathToPrompts"));
	ui.leVocab->setText(Settings::getS("Model/PathToVocab"));
	ui.leWavConfig->setText(Settings::getS("Model/PathToWavConfig"));

	ui.leConfig->setText(Settings::getS("Model/PathToConfig"));
	ui.leProto->setText(Settings::getS("Model/PathToProto"));
	ui.leTreeHed->setText(Settings::getS("Model/PathToTreeHed"));


	ui.leMkPhones0->setText(Settings::getS("Model/PathToMkPhones0"));
	ui.leMkPhones1->setText(Settings::getS("Model/PathToMkPhones1"));

	ui.leSamplePath->setText(Settings::getS("Model/PathToSamples"));
	ui.leGlobalDed->setText(Settings::getS("Model/PathToGlobalDed"));
	ui.leSilHed->setText(Settings::getS("Model/PathToSilHed"));
	ui.leMktriLed->setText(Settings::getS("Model/PathToMktriLed"));

	ui.leHmmOut->setText(Settings::getS("Model/PathToHmm"));
	ui.leTiedlist->setText(Settings::getS("Model/PathToTiedlist"));

	ui.leShadowLexicon->setText(Settings::getS("Model/PathToShadowLexicon"));
	ui.leShadowVocab->setText(Settings::getS("Model/PathToShadowVocab"));

	ui.leDict->setText(Settings::getS("Model/PathToDict"));
	ui.leDfa->setText(Settings::getS("Model/PathToDfa"));

	ui.leMonophones->setText(Settings::getS("Model/PathToPhonemes"));
	return true;
}

bool ModelSettings::reset()
{
	return init();
}

