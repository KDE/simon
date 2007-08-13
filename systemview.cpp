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


SystemView::SystemView(QWidget* parent): InlineWidget(tr("System"), QIcon(":/images/icons/computer.svg"), tr("Einstellungen, Protokolle, etc."), parent)
{
	setupUi(this);
	registerControl(new GeneralSettings(this));
	registerControl(new SoundSettings(this));
	registerControl(new Revert(this));

	connect(selectControl, SIGNAL(currentRowChanged(int)), this, SLOT(displayId(int)));
	connect(pbApply, SIGNAL(clicked()), this, SLOT(apply()));
	connect(pbReset, SIGNAL(clicked()), this, SLOT(reset()));

}

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

void SystemView::ok()
{
	
}


void SystemView::displayId(int id)
{
	if (id == -1) return; //none selected
	
	controls->setCurrentIndex(id);

	SystemWidget *sysWidget = dynamic_cast<SystemWidget*>(controls->widget(id));
	if (!sysWidget) return;
	help->setText(sysWidget->getHelp());
}

void SystemView::setupUi(QWidget *parent)
{
	help = new QTextEdit(parent);
	selectControl = new QListWidget(parent);
	QVBoxLayout *layLeft = new QVBoxLayout(0);

	selectControl->setMaximumWidth(250);
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

void SystemView::registerControl(SystemWidget* control)
{
	if (!control->init())
	{
		//something went wrong
		Logger::log("[ERR] "+tr("Konnte %1 nicht initiieren").arg(control->getTitle()));
		return;
	}
	controls->addWidget(control);
	
	QListWidgetItem *newItem = new QListWidgetItem(control->getIcon(), 
				control->getTitle(), selectControl);
}

void SystemView::deleteControl(SystemWidget* control)
{
	controls->removeWidget(control);
}

SystemView::~SystemView()
{
}


