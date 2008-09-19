//
// C++ Implementation: externalprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "commandsettings.h"
#include "../SimonLib/Settings/settings.h"

/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 * 
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
CommandSettings::CommandSettings(QWidget* parent): SystemWidget(i18n("Kommandos"), KIcon(":/images/icons/applications-system.svg"), i18n("Kommandos verwalten"), parent)
{
	ui.setupUi(this);
	guessChildTriggers(this);
	hide();

	connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	connect(ui.gbExe, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.urCommands, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	connect(ui.gbPlace, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.urPlace, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	connect(ui.gbTextMacro, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.urTextMacro, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	connect(ui.gbShortcut, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.urShortcut, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

	connect(ui.gbDesktopGrid, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.leDesktopGridTrigger, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
	connect(ui.cbDesktopGridRealTransparency, SIGNAL(toggled(bool)), this, SIGNAL(changed()));

	connect(ui.gbATIntegration, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.cbATIntegrationQtWorkarounds, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
	connect(ui.cbATIntegrationQtMenu, SIGNAL(toggled(bool)), this, SIGNAL(changed()));

	help = i18n("Konfigurieren Sie hier die Kommandos simons.\n\nSie können hier die einzelnen Typen konfigurieren und sogar (de-/)aktivieren.");
}

/**
 * \brief Returns true if all fields are filled out
 * \author Peter Grasch
 * @return True, if all mandatory fields are filled out
 */
bool CommandSettings::isComplete()
{
	bool exeOk = (!ui.gbExe->isChecked() || !ui.urCommands->url().isEmpty());
	bool placeOk = (!ui.gbPlace->isChecked() || !ui.urPlace->url().isEmpty());
	bool textMacroOk = (!ui.gbTextMacro->isChecked() || !ui.urTextMacro->url().isEmpty());
	bool shortcutOk = (!ui.gbShortcut->isChecked() || !ui.urShortcut->url().isEmpty());
	bool deskGridOk = (!ui.gbDesktopGrid->isChecked() || !ui.leDesktopGridTrigger->text().isEmpty());
	return (!ui.leTrigger->text().isEmpty() && exeOk && placeOk && textMacroOk && shortcutOk && deskGridOk);
}


/**
 * \brief Applys the changed settings
 * \author Peter Grasch
 * @return Success
 */
bool CommandSettings::apply()
{
	Settings::set("Commands/Dictation", ui.cbDictation->isChecked());
	Settings::set("Commands/UseGlobalTrigger", ui.cbUseTrigger->isChecked());

	Settings::set("Commands/Trigger", ui.leTrigger->text());
	Settings::set("Commands/Executable/Enabled", ui.gbExe->isChecked());
	Settings::set("Commands/Executable/PathToConfig", ui.urCommands->url().pathOrUrl());

	Settings::set("Commands/Place/Enabled", ui.gbPlace->isChecked());
	Settings::set("Commands/Place/PathToConfig", ui.urPlace->url().pathOrUrl());

	Settings::set("Commands/TextMacro/Enabled", ui.gbTextMacro->isChecked());
	Settings::set("Commands/TextMacro/PathToConfig", ui.urTextMacro->url().pathOrUrl());

	Settings::set("Commands/Shortcut/Enabled", ui.gbShortcut->isChecked());
	Settings::set("Commands/Shortcut/PathToConfig", ui.urShortcut->url().pathOrUrl());

	Settings::set("Commands/DesktopGrid/Enabled", ui.gbDesktopGrid->isChecked());
	Settings::set("Commands/DesktopGrid/Trigger", ui.leDesktopGridTrigger->text());
	Settings::set("Commands/DesktopGrid/RealTransparency", ui.cbDesktopGridRealTransparency->isChecked());

	Settings::set("Commands/ATIntegration/Enabled", ui.gbATIntegration->isChecked());
	Settings::set("Commands/ATIntegration/QtWorkarounds", ui.cbATIntegrationQtWorkarounds->isChecked());
	Settings::set("Commands/ATIntegration/QtMenuSupport", ui.cbATIntegrationQtMenu->isChecked());
// 	ActionManager::getInstance()->setupBackends();
	return true;
}

/**
 * \brief Resets the changed settings
 * \author Peter Grasch
 * @return Success
 */
bool CommandSettings::reset()
{
	return init();
}

/**
 * \brief Initialy loads the settings
 * \author Peter Grasch
 * @return Success
 */
bool CommandSettings::init()
{
	ui.cbDictation->setChecked(Settings::getB("Commands/Dictation"));
	ui.cbUseTrigger->setChecked(Settings::getB("Commands/UseGlobalTrigger"));
	ui.leTrigger->setText(Settings::getS("Commands/Trigger"));

	ui.gbExe->setChecked(Settings::getB("Commands/Executable/Enabled"));
	ui.urCommands->setPath(Settings::getS("Commands/Executable/PathToConfig"));

	ui.gbPlace->setChecked(Settings::getB("Commands/Place/Enabled"));
	ui.urPlace->setPath(Settings::getS("Commands/Place/PathToConfig"));

	ui.gbTextMacro->setChecked(Settings::getB("Commands/TextMacro/Enabled"));
	ui.urTextMacro->setPath(Settings::getS("Commands/TextMacro/PathToConfig"));

	ui.gbShortcut->setChecked(Settings::getB("Commands/Shortcut/Enabled"));
	ui.urShortcut->setPath(Settings::getS("Commands/Shortcut/PathToConfig"));

	ui.gbDesktopGrid->setChecked(Settings::getB("Commands/DesktopGrid/Enabled"));
	ui.leDesktopGridTrigger->setText(Settings::getS("Commands/DesktopGrid/Trigger"));
	ui.cbDesktopGridRealTransparency->setChecked(Settings::getB("Commands/DesktopGrid/RealTransparency"));

	ui.gbATIntegration->setChecked(Settings::getB("Commands/ATIntegration/Enabled"));
	ui.cbATIntegrationQtWorkarounds->setChecked(Settings::getB("Commands/ATIntegration/QtWorkarounds"));
	ui.cbATIntegrationQtMenu->setChecked(Settings::getB("Commands/ATIntegration/QtMenuSupport"));
	
	return true;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
CommandSettings::~CommandSettings()
{
}


