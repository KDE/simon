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

#include "commandsettings.h"
#include "actionmanager.h"

#include <simonscenarios/commandmanager.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/voiceinterfacecommand.h>

#include <QListWidget>
#include <QVariant>
#include <QPointer>
#include <QListWidgetItem>

#include <KAboutData>
#include <KMessageBox>
#include <KService>
#include <KServiceTypeTrader>
#include <KPageWidget>
#include <kgenericfactory.h>


K_PLUGIN_FACTORY( CommandSettingsFactory, 
			registerPlugin< CommandSettings >(); 
		)
        
K_EXPORT_PLUGIN( CommandSettingsFactory("simonlib") )

CommandSettings* CommandSettings::instance = NULL;
/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
CommandSettings::CommandSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent),
	forceChangeFlag(false),
	isChanged(false)
{
	if (instance) instance->deleteLater();

	instance = this;

	Q_UNUSED(args)

	ui.setupUi(this);

	KAboutData *about = new KAboutData(
				"commandsettings", "", ki18n("Command Settings"),
				"0.1", ki18n("Configuration about the Commands and the Selection of Plugins to use"), KAboutData::License_GPL);
#if KDE_IS_VERSION(4,0,80)
	about->setProgramIconName("fork");
#endif
	setAboutData( about );


	config = KSharedConfig::openConfig(KGlobal::mainComponent(),
					"simoncommandrc");

	QObject::connect(ui.cbUseDYM, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.sbMinimumConfidence, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
	QObject::connect(ui.fcFont, SIGNAL(fontSelected(const QFont&)), this, SLOT(slotChanged()));

	QObject::connect(ui.elbTriggers, SIGNAL(changed()), this, SLOT(slotChanged()));
	QObject::connect(ui.cbShowIcon, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	QObject::connect(ui.leVisibleTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.ibIcon, SIGNAL(iconChanged(QString)), this, SLOT(slotChanged()));

	QObject::connect(ui.lwActions, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(listActionsItemChanged(QListWidgetItem*, QListWidgetItem*)));

	load();
}


void CommandSettings::save()
{
	KConfigGroup cg(config, "");
	cg.writeEntry("MinimumConfidence", ui.sbMinimumConfidence->value());
	cg.writeEntry("UseDYM", ui.cbUseDYM->isChecked());
	if (storedFont != ui.fcFont->font()) {
		storedFont = ui.fcFont->font();
		cg.writeEntry("PluginBaseFont", storedFont);
		ScenarioManager::getInstance()->setPluginFont(storedFont);
	}
	storeCurrentlyDisplayedElement(getRowElementType(ui.lwActions->currentRow()));

	cg.writeEntry("ListBackTriggers", getListTriggers(CommandListElements::Back));
	cg.writeEntry("ListOneTriggers", getListTriggers(CommandListElements::One));
	cg.writeEntry("ListTwoTriggers", getListTriggers(CommandListElements::Two));
	cg.writeEntry("ListThreeTriggers", getListTriggers(CommandListElements::Three));
	cg.writeEntry("ListFourTriggers", getListTriggers(CommandListElements::Four));
	cg.writeEntry("ListFiveTriggers", getListTriggers(CommandListElements::Five));
	cg.writeEntry("ListSixTriggers", getListTriggers(CommandListElements::Six));
	cg.writeEntry("ListSevenTriggers", getListTriggers(CommandListElements::Seven));
	cg.writeEntry("ListEightTriggers", getListTriggers(CommandListElements::Eight));
	cg.writeEntry("ListNextTriggers", getListTriggers(CommandListElements::Next));
	cg.writeEntry("ListCancelTriggers", getListTriggers(CommandListElements::Cancel));

	cg.writeEntry("ListShowBackIcon", getListShowIcon(CommandListElements::Back));
	cg.writeEntry("ListShowOneIcon", getListShowIcon(CommandListElements::One));
	cg.writeEntry("ListShowTwoIcon", getListShowIcon(CommandListElements::Two));
	cg.writeEntry("ListShowThreeIcon", getListShowIcon(CommandListElements::Three));
	cg.writeEntry("ListShowFourIcon", getListShowIcon(CommandListElements::Four));
	cg.writeEntry("ListShowFiveIcon", getListShowIcon(CommandListElements::Five));
	cg.writeEntry("ListShowSixIcon", getListShowIcon(CommandListElements::Six));
	cg.writeEntry("ListShowSevenIcon", getListShowIcon(CommandListElements::Seven));
	cg.writeEntry("ListShowEightIcon", getListShowIcon(CommandListElements::Eight));
	cg.writeEntry("ListShowNextIcon", getListShowIcon(CommandListElements::Next));
	cg.writeEntry("ListShowCancelIcon", getListShowIcon(CommandListElements::Cancel));

	cg.writeEntry("ListBackIcon", getListIcon(CommandListElements::Back));
	cg.writeEntry("ListIconOne", getListIcon(CommandListElements::One));
	cg.writeEntry("ListIconTwo", getListIcon(CommandListElements::Two));
	cg.writeEntry("ListIconThree", getListIcon(CommandListElements::Three));
	cg.writeEntry("ListIconFour", getListIcon(CommandListElements::Four));
	cg.writeEntry("ListIconFive", getListIcon(CommandListElements::Five));
	cg.writeEntry("ListIconSix", getListIcon(CommandListElements::Six));
	cg.writeEntry("ListIconSeven", getListIcon(CommandListElements::Seven));
	cg.writeEntry("ListIconEight", getListIcon(CommandListElements::Eight));
	cg.writeEntry("ListIconNext", getListIcon(CommandListElements::Next));
	cg.writeEntry("ListCancelIcon", getListIcon(CommandListElements::Cancel));

	cg.writeEntry("ListBackVisibleTrigger", getListVisibleTrigger(CommandListElements::Back));
	cg.writeEntry("ListOneVisibleTrigger", getListVisibleTrigger(CommandListElements::One));
	cg.writeEntry("ListTwoVisibleTrigger", getListVisibleTrigger(CommandListElements::Two));
	cg.writeEntry("ListThreeVisibleTrigger", getListVisibleTrigger(CommandListElements::Three));
	cg.writeEntry("ListFourVisibleTrigger", getListVisibleTrigger(CommandListElements::Four));
	cg.writeEntry("ListFiveVisibleTrigger", getListVisibleTrigger(CommandListElements::Five));
	cg.writeEntry("ListSixVisibleTrigger", getListVisibleTrigger(CommandListElements::Six));
	cg.writeEntry("ListSevenVisibleTrigger", getListVisibleTrigger(CommandListElements::Seven));
	cg.writeEntry("ListEightVisibleTrigger", getListVisibleTrigger(CommandListElements::Eight));
	cg.writeEntry("ListNextVisibleTrigger", getListVisibleTrigger(CommandListElements::Next));
	cg.writeEntry("ListCancelVisibleTrigger", getListVisibleTrigger(CommandListElements::Cancel));

	cg.sync();
	KCModule::save();
	emit changed(false);
//	emit recognitionResultsFilterParametersChanged();
//
	ActionManager::getInstance()->retrieveRecognitionResultFilteringParameters();
//	ActionManager::getInstance()->setListInterfaceCommands(listInterfaceCommands);
}

void CommandSettings::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");

	float minimumConfidence = cg.readEntry("MinimumConfidence", 0.7f);
	ui.sbMinimumConfidence->setValue(minimumConfidence);

	ui.cbUseDYM->setChecked(cg.readEntry("UseDYM", true));
	storedFont = cg.readEntry("PluginBaseFont", QFont());
	ui.fcFont->setFont(storedFont);

	QStringList backTriggers, oneTriggers, twoTriggers, threeTriggers, fourTriggers,
		    fiveTriggers, sixTriggers, sevenTriggers, eightTriggers, nextTriggers,
		    cancelTriggers;
	
	backTriggers = cg.readEntry("ListBackTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Back));
	oneTriggers = cg.readEntry("ListOneTriggers", QStringList() << getListDefaultTrigger(CommandListElements::One));
	twoTriggers = cg.readEntry("ListTwoTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Two));
	threeTriggers = cg.readEntry("ListThreeTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Three));
	fourTriggers = cg.readEntry("ListFourTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Four));
	fiveTriggers = cg.readEntry("ListFiveTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Five));
	sixTriggers = cg.readEntry("ListSixTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Six));
	sevenTriggers = cg.readEntry("ListSevenTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Seven));
	eightTriggers = cg.readEntry("ListEightTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Eight));
	nextTriggers = cg.readEntry("ListNextTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Next));
	cancelTriggers = cg.readEntry("ListCancelTriggers", QStringList() << getListDefaultTrigger(CommandListElements::Cancel));

	bool showBackIcon, showOneIcon, showTwoIcon, showThreeIcon, showFourIcon,
	     showFiveIcon, showSixIcon, showSevenIcon, showEightIcon, 
	     showNextIcon, showCancelIcon;

	showBackIcon = cg.readEntry("ListShowBackIcon", getListDefaultShowIcon(CommandListElements::Back));
	showOneIcon = cg.readEntry("ListShowOneIcon", getListDefaultShowIcon(CommandListElements::One));
	showTwoIcon = cg.readEntry("ListShowTwoIcon", getListDefaultShowIcon(CommandListElements::Two));
	showThreeIcon = cg.readEntry("ListShowThreeIcon", getListDefaultShowIcon(CommandListElements::Three));
	showFourIcon = cg.readEntry("ListShowFourIcon", getListDefaultShowIcon(CommandListElements::Four));
	showFiveIcon = cg.readEntry("ListShowFiveIcon", getListDefaultShowIcon(CommandListElements::Five));
	showSixIcon = cg.readEntry("ListShowSixIcon", getListDefaultShowIcon(CommandListElements::Six));
	showSevenIcon = cg.readEntry("ListShowSevenIcon", getListDefaultShowIcon(CommandListElements::Seven));
	showEightIcon = cg.readEntry("ListShowEightIcon", getListDefaultShowIcon(CommandListElements::Eight));
	showNextIcon = cg.readEntry("ListShowNextIcon", getListDefaultShowIcon(CommandListElements::Next));
	showCancelIcon = cg.readEntry("ListShowCancelIcon", getListDefaultShowIcon(CommandListElements::Cancel));


	QString backIcon, oneIcon, twoIcon, threeIcon, fourIcon, fiveIcon, sixIcon, 
		sevenIcon, eightIcon, nextIcon, cancelIcon;
	backIcon = cg.readEntry("ListBackIcon", getListDefaultIcon(CommandListElements::Back));
	oneIcon = cg.readEntry("ListOneIcon", getListDefaultIcon(CommandListElements::One));
	twoIcon = cg.readEntry("ListTwoIcon", getListDefaultIcon(CommandListElements::Two));
	threeIcon = cg.readEntry("ListThreeIcon", getListDefaultIcon(CommandListElements::Three));
	fourIcon = cg.readEntry("ListFourIcon", getListDefaultIcon(CommandListElements::Four));
	fiveIcon = cg.readEntry("ListFiveIcon", getListDefaultIcon(CommandListElements::Five));
	sixIcon = cg.readEntry("ListSixIcon", getListDefaultIcon(CommandListElements::Six));
	sevenIcon = cg.readEntry("ListSevenIcon", getListDefaultIcon(CommandListElements::Seven));
	eightIcon = cg.readEntry("ListEightIcon", getListDefaultIcon(CommandListElements::Eight));
	nextIcon = cg.readEntry("ListNextIcon", getListDefaultIcon(CommandListElements::Next));
	cancelIcon = cg.readEntry("ListCancelIcon", getListDefaultIcon(CommandListElements::Cancel));

	QString backVisibleTrigger, oneVisibleTrigger, twoVisibleTrigger, threeVisibleTrigger,
		fourVisibleTrigger, fiveVisibleTrigger, sixVisibleTrigger, sevenVisibleTrigger,
		eightVisibleTrigger, nextVisibleTrigger, cancelVisibleTrigger;
	backVisibleTrigger = cg.readEntry("ListBackVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Back));
	oneVisibleTrigger = cg.readEntry("ListOneVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::One));
	twoVisibleTrigger = cg.readEntry("ListTwoVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Two));
	threeVisibleTrigger = cg.readEntry("ListThreeVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Three));
	fourVisibleTrigger = cg.readEntry("ListFourVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Four));
	fiveVisibleTrigger = cg.readEntry("ListFiveVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Five));
	sixVisibleTrigger = cg.readEntry("ListSixVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Six));
	sevenVisibleTrigger = cg.readEntry("ListSevenVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Seven));
	eightVisibleTrigger = cg.readEntry("ListEightVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Eight));
	nextVisibleTrigger = cg.readEntry("ListNextVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Next));
	cancelVisibleTrigger = cg.readEntry("ListCancelVisibleTrigger", getListDefaultVisibleTrigger(CommandListElements::Cancel));

	cg.sync();

	qDeleteAll(listInterfaceCommands);
	listInterfaceCommands.clear();

	registerVoiceInterfaceCommand(CommandListElements::Back, backTriggers, backVisibleTrigger, showBackIcon, backIcon);
	registerVoiceInterfaceCommand(CommandListElements::One, oneTriggers, oneVisibleTrigger, showOneIcon, oneIcon);
	registerVoiceInterfaceCommand(CommandListElements::Two, twoTriggers, twoVisibleTrigger, showTwoIcon, twoIcon);
	registerVoiceInterfaceCommand(CommandListElements::Three, threeTriggers, threeVisibleTrigger, showThreeIcon, threeIcon);
	registerVoiceInterfaceCommand(CommandListElements::Four, fourTriggers, fourVisibleTrigger, showFourIcon, fourIcon);
	registerVoiceInterfaceCommand(CommandListElements::Five, fiveTriggers, fiveVisibleTrigger, showFiveIcon, fiveIcon);
	registerVoiceInterfaceCommand(CommandListElements::Six, sixTriggers, sixVisibleTrigger, showSixIcon, sixIcon);
	registerVoiceInterfaceCommand(CommandListElements::Seven, sevenTriggers, sevenVisibleTrigger, showSevenIcon, sevenIcon);
	registerVoiceInterfaceCommand(CommandListElements::Eight, eightTriggers, eightVisibleTrigger, showEightIcon, eightIcon);
	registerVoiceInterfaceCommand(CommandListElements::Next, nextTriggers, nextVisibleTrigger, showNextIcon, nextIcon);
	registerVoiceInterfaceCommand(CommandListElements::Cancel, cancelTriggers, cancelVisibleTrigger, showCancelIcon, cancelIcon);


	ui.lwActions->setCurrentRow(0);
	displayListElement(CommandListElements::Back);
	emit changed(false);
	KCModule::load();
}

CommandListElements::Element CommandSettings::getRowElementType(int row)
{
	switch (row)
	{
		case 0: return CommandListElements::Back;
		case 1: return CommandListElements::One;
		case 2: return CommandListElements::Two;
		case 3: return CommandListElements::Three;
		case 4: return CommandListElements::Four;
		case 5: return CommandListElements::Five;
		case 6: return CommandListElements::Six;
		case 7: return CommandListElements::Seven;
		case 8: return CommandListElements::Eight;
		case 9: return CommandListElements::Next;
		default: return CommandListElements::Cancel;
	}
}

QString CommandSettings::getListSelectionId(CommandListElements::Element type)
{
	switch (type)
	{
		case CommandListElements::Back: return "listSelectBack";
		case CommandListElements::One: return "listSelectOne";
		case CommandListElements::Two: return "listSelectTwo";
		case CommandListElements::Three: return "listSelectThree";
		case CommandListElements::Four: return "listSelectFour";
		case CommandListElements::Five: return "listSelectFive";
		case CommandListElements::Six: return "listSelectSix";
		case CommandListElements::Seven: return "listSelectSeven";
		case CommandListElements::Eight: return "listSelectEight";
		case CommandListElements::Next: return "listSelectNext";
		case CommandListElements::Cancel: return "listSelectCancel";
	}

	return "";
}

QString CommandSettings::getListDefaultIcon(CommandListElements::Element type)
{
	if (type == CommandListElements::Cancel) return "dialog-cancel";

	return "view-choose";
}

bool CommandSettings::getListDefaultShowIcon(CommandListElements::Element type)
{
	if (type == CommandListElements::Cancel) return true;

	return false;
}

QString CommandSettings::getListSelectionDescription(CommandListElements::Element type)
{
	switch (type)
	{
		case CommandListElements::Back: return i18n("Selects the \"Back\" option");
		case CommandListElements::One: return i18n("Selects the \"One\" option");
		case CommandListElements::Two: return i18n("Selects the \"Two\" option");
		case CommandListElements::Three: return i18n("Selects the \"Three\" option");
		case CommandListElements::Four: return i18n("Selects the \"Four\" option");
		case CommandListElements::Five: return i18n("Selects the \"Five\" option");
		case CommandListElements::Six: return i18n("Selects the \"Six\" option");
		case CommandListElements::Seven: return i18n("Selects the \"Seven\" option");
		case CommandListElements::Eight: return i18n("Selects the \"Eight\" option");
		case CommandListElements::Next: return i18n("Selects the \"Next\" option");
		case CommandListElements::Cancel: return i18n("Selects the \"Cancel\" option");
	}

	return "";
}

QString CommandSettings::getListDefaultTrigger(CommandListElements::Element type)
{
	switch (type)
	{
		case CommandListElements::Back: return i18n("Zero");
		case CommandListElements::One: return i18n("One");
		case CommandListElements::Two: return i18n("Two");
		case CommandListElements::Three: return i18n("Three");
		case CommandListElements::Four: return i18n("Four");
		case CommandListElements::Five: return i18n("Five");
		case CommandListElements::Six: return i18n("Six");
		case CommandListElements::Seven: return i18n("Seven");
		case CommandListElements::Eight: return i18n("Eight");
		case CommandListElements::Next: return i18n("Nine");
		case CommandListElements::Cancel: return i18n("Cancel");
	}

	return "";
}

QString CommandSettings::getListDefaultVisibleTrigger(CommandListElements::Element type)
{
	switch (type)
	{
		case CommandListElements::Back: return "0";
		case CommandListElements::One: return "1";
		case CommandListElements::Two: return "2";
		case CommandListElements::Three: return "3";
		case CommandListElements::Four: return "4";
		case CommandListElements::Five: return "5";
		case CommandListElements::Six: return "6";
		case CommandListElements::Seven: return "7";
		case CommandListElements::Eight: return "8";
		case CommandListElements::Next: return "9";
		case CommandListElements::Cancel: return i18n("Cancel");
	}

	return "";
}


void CommandSettings::storeCurrentlyDisplayedElement(CommandListElements::Element type)
{
	QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
	listInterfaceCommands.remove(type);
	qDeleteAll(commands);

	registerVoiceInterfaceCommand(type, ui.elbTriggers->items(), ui.leVisibleTrigger->text(),
			ui.cbShowIcon->isChecked(), ui.ibIcon->icon());
}

void CommandSettings::listActionsItemChanged(QListWidgetItem *newItem, QListWidgetItem *item)
{
	if (!item || !newItem || (ui.lwActions->row(item) == ui.lwActions->row(newItem))) return;

	//store current item
	storeCurrentlyDisplayedElement(getRowElementType(ui.lwActions->row(item)));

	//display new item
	displayListElement(getRowElementType(ui.lwActions->row(newItem)));
}

QStringList CommandSettings::getListTriggers(CommandListElements::Element type)
{
	QStringList out;

	QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);

	foreach (VoiceInterfaceCommand* c, commands)
		out << c->getTrigger();

	return out;
}

bool CommandSettings::getListShowIcon(CommandListElements::Element type)
{
	QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
	if (commands.isEmpty())
		return getListDefaultShowIcon(type);
	return commands[0]->showIcon();
}

QString CommandSettings::getListIcon(CommandListElements::Element type)
{
	QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
	if (commands.isEmpty())
		return getListDefaultIcon(type);
	return commands[0]->getIconSrc();
}

QString CommandSettings::getListVisibleTrigger(CommandListElements::Element type)
{
	QList<VoiceInterfaceCommand*> commands = listInterfaceCommands.values(type);
	if (commands.isEmpty())
		return getListDefaultTrigger(type);
	return commands[0]->visibleTrigger();
}



void CommandSettings::displayListElement(CommandListElements::Element type)
{
	ui.cbShowIcon->setChecked(getListShowIcon(type));
	ui.ibIcon->setIcon(getListIcon(type));
	ui.leVisibleTrigger->setText(getListVisibleTrigger(type));
	ui.elbTriggers->setItems(getListTriggers(type));
}

void CommandSettings::registerVoiceInterfaceCommand(CommandListElements::Element element, 
		const QStringList& triggers, const QString& visibleTrigger, bool showIcon, const QString& iconSrc)
{
	foreach (const QString& trigger, triggers)
	{
		listInterfaceCommands.insertMulti(element, new VoiceInterfaceCommand(NULL, trigger, iconSrc,
					getListSelectionDescription(element), getListSelectionId(element), 
					0, 0, visibleTrigger, showIcon, false));
	}

}

float CommandSettings::minimumConfidence()
{
	return ui.sbMinimumConfidence->value();
}

bool CommandSettings::useDYM()
{
	return ui.cbUseDYM->isChecked();
}

QFont CommandSettings::pluginBaseFont()
{
	return storedFont;
}

void CommandSettings::defaults()
{
	KCModule::defaults();

	KConfigGroup cg(config, "");
	cg.writeEntry("MinimumConfidence", 0.7f);
	cg.writeEntry("UseDYM", true);
	cg.writeEntry("PluginBaseFont", QFont());
	ScenarioManager::getInstance()->setPluginFont(QFont());

	cg.deleteEntry("ListBackTriggers");
	cg.deleteEntry("ListOneTriggers");
	cg.deleteEntry("ListTwoTriggers");
	cg.deleteEntry("ListThreeTriggers");
	cg.deleteEntry("ListFourTriggers");
	cg.deleteEntry("ListFiveTriggers");
	cg.deleteEntry("ListSixTriggers");
	cg.deleteEntry("ListSevenTriggers");
	cg.deleteEntry("ListEightTriggers");
	cg.deleteEntry("ListNextTriggers");
	cg.deleteEntry("ListCancelTriggers");

	cg.deleteEntry("ListShowBackIcon");
	cg.deleteEntry("ListShowOneIcon");
	cg.deleteEntry("ListShowTwoIcon");
	cg.deleteEntry("ListShowThreeIcon");
	cg.deleteEntry("ListShowFourIcon");
	cg.deleteEntry("ListShowFiveIcon");
	cg.deleteEntry("ListShowSixIcon");
	cg.deleteEntry("ListShowSevenIcon");
	cg.deleteEntry("ListShowEightIcon");
	cg.deleteEntry("ListShowNextIcon");
	cg.deleteEntry("ListShowCancelIcon");

	cg.deleteEntry("ListBackIcon");
	cg.deleteEntry("ListIconOne");
	cg.deleteEntry("ListIconTwo");
	cg.deleteEntry("ListIconThree");
	cg.deleteEntry("ListIconFour");
	cg.deleteEntry("ListIconFive");
	cg.deleteEntry("ListIconSix");
	cg.deleteEntry("ListIconSeven");
	cg.deleteEntry("ListIconEight");
	cg.deleteEntry("ListIconNext");
	cg.deleteEntry("ListCancelIcon");

	cg.deleteEntry("ListBackVisibleTrigger");
	cg.deleteEntry("ListOneVisibleTrigger");
	cg.deleteEntry("ListTwoVisibleTrigger");
	cg.deleteEntry("ListThreeVisibleTrigger");
	cg.deleteEntry("ListFourVisibleTrigger");
	cg.deleteEntry("ListFiveVisibleTrigger");
	cg.deleteEntry("ListSixVisibleTrigger");
	cg.deleteEntry("ListSevenVisibleTrigger");
	cg.deleteEntry("ListEightVisibleTrigger");
	cg.deleteEntry("ListNextVisibleTrigger");
	cg.deleteEntry("ListCancelVisibleTrigger");

	cg.sync();
	load();
	forceChangeFlag = true;
	save();
}


void CommandSettings::slotChanged()
{
	emit changed(true);
}

QHash<CommandListElements::Element, VoiceInterfaceCommand*> CommandSettings::getListInterfaceCommands()
{
	return listInterfaceCommands;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
CommandSettings::~CommandSettings()
{
	instance = NULL;
}


