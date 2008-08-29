/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <KUrl>
#include "newcommand.h"

#include "Commands/Executable/executablecommand.h"
#include "Commands/Executable/ImportProgram/importprogramwizard.h"
#include "Commands/Place/placecommand.h"
#include "Commands/Place/ImportPlace/importplacewizard.h"
#include "Commands/Shortcut/shortcutcommand.h"
#include "Commands/TextMacro/textmacrocommand.h"

NewCommand::NewCommand(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	ui.ksShortcut->setCheckAgainstStandardShortcuts(false);
	
	checkIfComplete();
	connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(setWindowTitleToCommandName(QString)));
	connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
	connect(ui.cbImportProgram, SIGNAL(clicked()), this, SLOT(showImportProgramWizard()));
	connect(ui.cbImportPlace, SIGNAL(clicked()), this, SLOT(showImportPlaceWizard()));
}


void NewCommand::init(Command *command)
{
	if (!command) return;
	ui.leTrigger->setText(command->getTrigger());
	ui.ibIcon->setIcon(command->getIconSrc());

	if (dynamic_cast<ExecutableCommand*>(command))
	{
		ExecutableCommand *exe = dynamic_cast<ExecutableCommand*>(command);

		ui.cbType->setCurrentIndex(0); //Executable menu
		ui.leExecutable->setText(exe->getExecutable());
		ui.leWorkingDirectory->setText(exe->getWorkingDirectory().prettyUrl());
	}
	if (dynamic_cast<PlaceCommand*>(command))
	{
		PlaceCommand *place = dynamic_cast<PlaceCommand*>(command);

		ui.cbType->setCurrentIndex(1); //Place menu
		ui.leUrl->setText(place->getURL().toString());
	}
	if (dynamic_cast<ShortcutCommand*>(command))
	{
		ShortcutCommand *shortcut = dynamic_cast<ShortcutCommand*>(command);
		ui.cbType->setCurrentIndex(2); //Shortcut menu
		ui.ksShortcut->setKeySequence(shortcut->getShortcut());
	}
	if (dynamic_cast<TextMacroCommand*>(command))
	{
		TextMacroCommand *macro = dynamic_cast<TextMacroCommand*>(command);
		ui.cbType->setCurrentIndex(3); //Macro menu
		ui.teMacroText->setPlainText(macro->getText());
	}
}

void NewCommand::checkIfComplete()
{
	QPushButton *okBtn = ui.bbDialog->button(QDialogButtonBox::Ok);
	if (!okBtn) return;
	
	okBtn->setEnabled(!ui.leTrigger->text().isEmpty());
}

void NewCommand::setWindowTitleToCommandName(QString name)
{
	if (!name.isEmpty())
		setWindowTitle(tr("Kommando: %1").arg(name));
	else setWindowTitle(tr("Kommando"));
}

void NewCommand::showImportProgramWizard()
{
	ImportProgramWizard *import = new ImportProgramWizard(this);
	connect (import, SIGNAL(commandCreated(Command*)), this, SLOT(init(Command*)));
	import->exec();
	import->deleteLater();
}

void NewCommand::showImportPlaceWizard()
{
	ImportPlaceWizard *import = new ImportPlaceWizard(this);
	connect (import, SIGNAL(commandCreated(Command*)), this, SLOT(init(Command*)));
	import->exec();
	import->deleteLater();
}

Command* NewCommand::newCommand()
{
	if (QDialog::exec())
	{
		//creating
		Command *command=0;
		
		int type = ui.cbType->currentIndex();
		switch (type)
		{
			case 0: //Program
				command = new ExecutableCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
							ui.leExecutable->text(), ui.leWorkingDirectory->text());
				break;
				
			case 1: //place
				command = new PlaceCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
							KUrl(ui.leUrl->text()));
				break;
				
			case 2: //shortcut
				command = new ShortcutCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
							ui.ksShortcut->keySequence());
				break;
				
			case 3: //textmacro
				command = new TextMacroCommand(ui.leTrigger->text(), ui.ibIcon->icon(),
							ui.teMacroText->toPlainText());
				break;
		}
		ui.ksShortcut->clearKeySequence();
		return command;
	}
	ui.ksShortcut->clearKeySequence();
	return 0;
}

NewCommand::~NewCommand()
{

}
