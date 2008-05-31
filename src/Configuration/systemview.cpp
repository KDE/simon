//
// C++ Implementation: systemview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "simonlistwidget.h"
#include <QMessageBox>
#include <QDebug>
#include "systemview.h"
#include "systemwidget.h"
#include "generalsettings.h"
#include "soundsettings.h"
#include "logview.h"
#include "externalprogrammanager.h"
#include "networksettings.h"
#include "internetextensionsettings.h"
#include "passwordsettings.h"

#include "../ModelManagement/modelsettings.h"
#include "../ModelManagement/Grammar/grammarsettings.h"
#include "../Actions/Commands/commandsettings.h"
#include "../SimonLib/SimonInfo/simoninfo.h"
#include "../SimonLib/Logging/logger.h"

/**
 *  \author Peter Grasch
 *  \brief Constructor - inits the ui and registers the controls
 * @param parent the parent of the widget
 */
SystemView::SystemView(QWidget* parent): InlineWidget(tr("System"), QIcon(":/images/icons/computer.svg"), tr("Einstellungen, Protokolle, etc."), parent)
{
	ui.setupUi(this);
	hide();
	registerControl(new GeneralSettings(this));
	registerControl(new InternetExtensionSettings(this));
	registerControl(new PasswordSettings(this));
	registerControl(new ModelSettings(this));
	registerControl(new GrammarSettings(this));
	registerControl(new SoundSettings(this));
	registerControl(new NetworkSettings(this));
	registerControl(new CommandSettings(this));
	registerControl(new LogView(this));
	registerControl(new ExternalProgramManager(this));

	connect(ui.lwMenu, SIGNAL(currentRowChanged(int)), this, SLOT(displayId(int)));
	connect(ui.pbApply, SIGNAL(clicked()), this, SLOT(apply()));
	connect(ui.pbReset, SIGNAL(clicked()), this, SLOT(reset()));

	ui.lwMenu->setIconSize(QSize(24,24));
	
	guessChildTriggers((QObject*)this);
}

/**
 * \brief Tells all the controls to apply the changes
 * \author Peter Grasch
 */
void SystemView::apply()
{
	SystemWidget *currentControl;
	for (int i=0; i < ui.swControls->count(); i++)
	{
		currentControl = dynamic_cast<SystemWidget*>(ui.swControls->widget(i));
		if (currentControl)
			if (!currentControl->apply())
				Logger::log("[ERR] "+tr("Konnte Änderungen in \"%1\" nicht speichern.").arg(currentControl->getTitle()));
	}
	SimonInfo::showMessage(tr("Einstellungen übernommen"),3000);
}

/**
 * \brief Tells all the controls to reset the changes
 * \author Peter Grasch
 */
void SystemView::reset()
{
	SystemWidget *currentControl;
	for (int i=0; i < ui.swControls->count(); i++)
	{
		currentControl = dynamic_cast<SystemWidget*>(ui.swControls->widget(i));
		if (currentControl)
			if (!currentControl->reset())
				Logger::log("[ERR] "+tr("Konnte Änderungen in \"%1\" nicht zurücknehmen.").arg(currentControl->getTitle()));
	}
}

/**
 * \brief Displays the control at the position <id>
 * \author Peter Grasch
 * @param id The id to display
 */
void SystemView::displayId(int id)
{
	if (id == -1) return; //none selected
	id++; //skip the "Welcome page"
	ui.swControls->setCurrentIndex(id);
	SystemWidget *sysWidget = dynamic_cast<SystemWidget*>(ui.swControls->widget(id));
	if (!sysWidget) return;
	ui.tbHelp->setText(sysWidget->getHelp());
}

/**
 * \brief Registers the given SystemWidget as a control in the view and displays it
 * \author Peter Grasch
 * @param control The control to register
 */
void SystemView::registerControl(SystemWidget* control)
{
	ui.swControls->addWidget(control);
	connect(this, SIGNAL(guiAction(QString)), control, SLOT(doAction(QString)));
	if (!control->init())
	{
		//something went wrong
		Logger::log("[ERR] "+tr("Konnte %1 nicht initiieren").arg(control->getTitle()));
		QMessageBox::critical(this, tr("Schwerwiegender Fehler aufgetreten"), tr("Es ist ein Schwerwiegender Fehler aufgetreten.\n\nEs konnten nicht alle Elemente der Konfiguration initalisiert werden. Dies ist meist auf eine fehlerhaft Konfiguration zurückzuführen.\n\nBitte kontrollieren Sie ihre Einstellungen (vor allem im Bezug auf mögliche vorangegangene Fehlermeldungen) und starten sie simon danach neu."));
	}
	
	//item gets automatically added to the list widget if given the parent
	new QListWidgetItem(control->getIcon(), control->getTitle(), ui.lwMenu);
}

/**
 * \brief Removes the control
 * @param control The control to remove
 */
void SystemView::deleteControl(SystemWidget* control)
{
	ui.swControls->removeWidget(control);
}

SystemView::~SystemView()
{
	for (int i=0; i < ui.swControls->count(); i++)
	{
		ui.swControls->widget(i)->deleteLater();
	}
}

