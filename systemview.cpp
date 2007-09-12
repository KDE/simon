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
#include "systemview.h"
#include "systemwidget.h"
#include <QTextEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "generalsettings.h"
#include "soundsettings.h"
#include "revert.h"
#include "logger.h"
#include "logview.h"
#include "externalprogrammanager.h"
#include "networksettings.h"
#include "commandsettings.h"
#include "shortcutsettings.h"

/**
 *  \author Peter Grasch
 *  \brief Constructor - inits the ui and registers the controls
 * @param parent the parent of the widget
 */
SystemView::SystemView(ShortcutControl *shortcutctrl, QWidget* parent): InlineWidget(tr("System"), QIcon(":/images/icons/computer.svg"), tr("Einstellungen, Protokolle, etc."), parent)
{
	setupUi(this);
	CommandSettings *commandsSettings = new CommandSettings(this);
	registerControl(new GeneralSettings(this));
	registerControl(new SoundSettings(this));
	registerControl(new NetworkSettings(this));
	registerControl(commandsSettings);
	registerControl(new ShortcutSettings(shortcutctrl, this));
	registerControl(new LogView(this));
	registerControl(new ExternalProgramManager(this));
	registerControl(new Revert(this));

	connect(selectControl, SIGNAL(currentRowChanged(int)), this, SLOT(displayId(int)));
	connect(pbApply, SIGNAL(clicked()), this, SLOT(apply()));
	connect(pbReset, SIGNAL(clicked()), this, SLOT(reset()));
    
    connect(commandsSettings, SIGNAL(commandsChanged()), this, SIGNAL(commandsChanged()));
}

/**
 * \brief Tells all the controls to apply the changes
 * \author Peter Grasch
 */
void SystemView::apply()
{
	SystemWidget *currentControl;
	for (int i=0; i < controls->count(); i++)
	{
		currentControl = dynamic_cast<SystemWidget*>(controls->widget(i));
		if (currentControl)
			if (!currentControl->apply())
				Logger::log("[ERR] "+tr("Konnte Änderungen in \"%1\" nicht speichern.").arg(currentControl->getTitle()));
	}
}

/**
 * \brief Tells all the controls to reset the changes
 * \author Peter Grasch
 */
void SystemView::reset()
{
	SystemWidget *currentControl;
	for (int i=0; i < controls->count(); i++)
	{
		currentControl = dynamic_cast<SystemWidget*>(controls->widget(i));
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
	
	SystemWidget *sysWidget = dynamic_cast<SystemWidget*>(controls->widget(id));
	if (!sysWidget) return;
	
	controls->setCurrentIndex(id);
	
	help->setText(sysWidget->getHelp());
}

/**
 * \brief Sets up the ui
 * \author Peter Grasch
 * @param parent the parent of the widgets
 */
void SystemView::setupUi(QWidget *parent)
{
	help = new QTextEdit(parent);
	selectControl = new QListWidget(parent);
	QVBoxLayout *layLeft = new QVBoxLayout(0);

	selectControl->setMaximumWidth(250);
        selectControl->setIconSize(QSize(24,24));
	help->setMaximumWidth(250);
	layLeft->addWidget(selectControl);
	layLeft->addWidget(help);
	
	pbApply = new QPushButton(QIcon(":/images/icons/document-save.svg"),
				  tr("Übernehmen"), parent);
	pbReset = new QPushButton(QIcon(":/images/icons/gtk-undo-ltr.svg"), 
				  tr("Zurücksetzen"), parent);
	
	QHBoxLayout *buttonsLay = new QHBoxLayout();
	buttonsLay->addWidget(pbReset);
	buttonsLay->addWidget(pbApply);
	
	layLeft->addLayout(buttonsLay);

	
	QHBoxLayout *layout = new QHBoxLayout(parent);
	controls = new QStackedLayout();
	QVBoxLayout *controlLay = new QVBoxLayout();
	controlLay->addLayout(controls);
	controlLay->addStretch();
	

	help->setReadOnly(true);

	layout->addLayout(layLeft);
	layout->addLayout(controlLay);
	setLayout(layout);
	hide();
}

/**
 * \brief Registers the given SystemWidget as a control in the view and displays it
 * \author Peter Grasch
 * @param control The control to register
 */
void SystemView::registerControl(SystemWidget* control)
{
	controls->addWidget(control);
	if (!control->init())
	{
		//something went wrong
		Logger::log("[ERR] "+tr("Konnte %1 nicht initiieren").arg(control->getTitle()));
		return;
	}
	
	//item gets automatically added to the list widget if given the parent
	//please ignore this compiler warning and
	//DO NOT REMOVE THIS
	QListWidgetItem *newItem = new QListWidgetItem(control->getIcon(), control->getTitle(), selectControl);

}

/**
 * \brief Removes the control
 * @param control The control to remove
 */
void SystemView::deleteControl(SystemWidget* control)
{
	controls->removeWidget(control);
}

SystemView::~SystemView()
{
}


