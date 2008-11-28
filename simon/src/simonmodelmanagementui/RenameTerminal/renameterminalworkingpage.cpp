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


#include "renameterminalworkingpage.h"
#include "renameterminal.h"

RenameTerminalWorkingPage::RenameTerminalWorkingPage(QWidget *parent)
 : QWizardPage(parent)
{
	complete = false;
	renameTerminal = new RenameTerminal(this);
	connect(renameTerminal, SIGNAL(progress(int)), this, SLOT(displayProgress(int)));
	connect(renameTerminal, SIGNAL(done()), this, SLOT(finish()));
	ui.setupUi(this);
	setTitle(i18n("Renaming terminal..."));
}

void RenameTerminalWorkingPage::displayProgress(int progress)
{
	ui.pgProgress->setValue(progress);
}

void RenameTerminalWorkingPage::finish()
{
	complete = true;
	emit completeChanged();
	emit done();
}

void RenameTerminalWorkingPage::initializePage()
{
	//do the work
	renameTerminal->setOldName(field("renameTerminal").toString());

	renameTerminal->setNewName(field("renameNewName").toString());
	renameTerminal->setIncludeShadow((field("renameIncludeShadow").toBool()));
	renameTerminal->setIncludeGrammar(field("renameIncludeGrammar").toBool());
	renameTerminal->start();
}


RenameTerminalWorkingPage::~RenameTerminalWorkingPage()
{
}


