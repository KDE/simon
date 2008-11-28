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


#include "mergeterminalsselectterminalspage.h"
#include "wordlistmanager.h"

MergeTerminalsSelectTerminalsPage::MergeTerminalsSelectTerminalsPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle(i18n("Choose Terminals"));
	registerField("newName*", ui.leNewTerminal);
	registerField("terminalA*", ui.lwA, "currentText", SIGNAL(currentTextChanged(QString)));
	registerField("terminalB*", ui.lwB, "currentText", SIGNAL(currentTextChanged(QString)));
	registerField("includeShadow", ui.cbIncludeShadow);
	registerField("includeGrammar", ui.cbIncludeGrammar);
}

void MergeTerminalsSelectTerminalsPage::initializePage()
{
	QStringList availableTerminals;
	availableTerminals = WordListManager::getInstance()->getTerminals(true);
	ui.lwA->clear();
	ui.lwB->clear();
	ui.lwA->addItems(availableTerminals);
	ui.lwB->addItems(availableTerminals);
}


MergeTerminalsSelectTerminalsPage::~MergeTerminalsSelectTerminalsPage()
{
}


