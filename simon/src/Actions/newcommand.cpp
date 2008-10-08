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

#include "newcommand.h"

#include <KUrl>
#include <KMessageBox>
#include <KKeySequenceWidget>
#include <KDialogButtonBox>
#include "Commands/commandpluginbase/command.h"
#include "Commands/commandpluginbase/createcommandwidget.h"
#include "actionmanager.h"

// #include "Commands/Executable/executablecommand.h"
// #include "Commands/Executable/ImportProgram/importprogramwizard.h"
// #include "Commands/Place/placecommand.h"
// #include "Commands/Place/ImportPlace/importplacewizard.h"
// #include "Commands/Shortcut/shortcutcommand.h"
// #include "Commands/TextMacro/textmacrocommand.h"

NewCommand::NewCommand(QWidget *parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);
	setMainWidget( widget );
	setCaption( i18n("Kommando") );
	
	//ui.ksShortcut->setCheckForConflictsAgainst(KKeySequenceWidget::None);
	
	checkIfComplete();
	connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(setWindowTitleToCommandName(QString)));
	connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	
	commandCreaters=0;
}

bool NewCommand::registerCreators(QList<CreateCommandWidget*>* commandCreaters)
{
	if (this->commandCreaters)
		delete this->commandCreaters;
	
	foreach (CreateCommandWidget *widget, *commandCreaters)
	{
		ui.cbType->addItem(widget->windowIcon(), widget->windowTitle());
		ui.swCommandCreaters->addWidget(widget);
		connect(widget, SIGNAL(complete(bool)), this, SLOT(slotPluginComplete(bool)));
	}
	
	this->commandCreaters = commandCreaters;
	return true;
}
/**
 * \brief En-/disables the Ok-Button; Takes general settings into account
 * \author Peter Grasch
 * 
 * This calls checkIfComplete() to check the general command-parameters
 * but sets the button also to disabled if the plugin reports it is not yet
 * complete
 *
 */
void NewCommand::slotPluginComplete(bool complete)
{
	checkIfComplete();
	if ((ui.swCommandCreaters->currentWidget() == sender()) && (!complete))
		enableButtonOk(false);
}

void NewCommand::init(Command *command)
{
	if (!command) return;
	
	ui.leTrigger->setText(command->getTrigger());
	ui.ibIcon->setIcon(command->getIconSrc());

	bool found=false;
	int i=0;
	foreach (CreateCommandWidget *widget, *commandCreaters)
	{
		if (widget->init(command))
		{
			found=true;
			ui.cbType->setCurrentIndex(i);
			ui.swCommandCreaters->setCurrentIndex(i);
		}
		i++;
	}
	if (!found)
		KMessageBox::error(this, i18n("Konnte Kommandotyp nicht bestimmen."));
}

void NewCommand::checkIfComplete()
{
	enableButtonOk(!ui.leTrigger->text().isEmpty());
}

void NewCommand::setWindowTitleToCommandName(QString name)
{
	if (!name.isEmpty())
		setCaption(i18n("Kommando: %1", name));
	else setCaption(i18n("Kommando"));
}

Command* NewCommand::newCommand()
{
	if (KDialog::exec())
	{
		//creating
		CreateCommandWidget *creater = dynamic_cast<CreateCommandWidget*>(ui.swCommandCreaters->currentWidget());
		Q_ASSERT(creater);
		
		if (!creater) return 0;
		
		return creater->createCommand(ui.leTrigger->text(), ui.ibIcon->icon());
		
// 		int type = ui.cbType->currentIndex();
// 		switch (type)
// 		{
// 			case 0: //Program
// 				command = new ExecutableCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
// 							ui.urExecutable->url().path(), ui.urWorkingDirectory->url());
// 				break;
// 				
// 			case 1: //place
// 				command = new PlaceCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
// 							ui.urUrl->url());
// 				break;
// 				
// 			case 2: //shortcut
// 				command = new ShortcutCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
// 							ui.ksShortcut->keySequence());
// 				break;
// 				
// 			case 3: //textmacro
// 				command = new TextMacroCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
// 							ui.teMacroText->toPlainText());
// 				break;
// 		}
// 		ui.ksShortcut->clearKeySequence();
// 		return command;
	}
// 	ui.ksShortcut->clearKeySequence();
	return 0;
}

NewCommand::~NewCommand()
{
	delete commandCreaters;
}
