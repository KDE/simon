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

ModelSettings::ModelSettings(QWidget* parent): SystemWidget(i18n("Modelleinstellungen"), KIcon("applications-education-language"), i18n("Hier können Sie Einstellungen rund um das Sprachmodell einstellen"), parent)
{
	ui.setupUi(this);
	
	connect(ui.urLexicon, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urGrammar, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urPrompts, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urVocab, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urWavConfig, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urConfig, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urProto, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urTreeHed, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urMkPhones0, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urMkPhones1, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urSamplePath, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urGlobalDed, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urSilHed, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urMktriLed, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urHmmOut, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urTiedlist, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urShadowLexicon, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.urShadowVocab, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	help = i18n("Hier können Sie Pfade zu den Sprachmodell-Dateien festlegen und Nachbearbeitungs-Ketten konfigurieren.\n\nAchtung: Unbedachte Änderungen in diesem Modul können die Funktionsfähigkeit von simon negativ beeinflussen");
}


ModelSettings::~ModelSettings()
{}




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

	Settings::set("Model/ProcessingFilters", ui.elbCommands->items().join(" && "));

	return true;
}



bool ModelSettings::init()
{
	ui.sbSamplerate->setValue(Settings::getI("Model/Samplerate"));
	ui.sbChannels->setValue(Settings::getI("Model/Channels"));

	ui.cbProcessInternal->setChecked(Settings::getB("Model/ProcessInternal"));

	QStringList filters = Settings::getS("Model/ProcessingFilters").split("&&", QString::SkipEmptyParts);
	
	ui.elbCommands->insertStringList(filters);

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

