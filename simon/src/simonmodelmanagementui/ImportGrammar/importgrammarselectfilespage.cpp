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


#include "importgrammarselectfilespage.h"
#include <KEditListBox>

//todo: document
//ambiguous words and words with more than one meaning are still ignored when using the "Also include unknown constructs" option; This is not a bug!
ImportGrammarSelectFilesPage::ImportGrammarSelectFilesPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Eingabedateien"));
	ui.setupUi(this);

	ui.elbFiles->setCustomEditor(*(new KEditListBox::CustomEditor(ui.urFileToAdd, ui.urFileToAdd->lineEdit())));
	
	registerField("files*", ui.elbFiles, "items", SIGNAL(changed()));
	registerField("includeUnknown", ui.cbIncludeUnknown);
}

void ImportGrammarSelectFilesPage::cleanupPage()
{
	ui.elbFiles->clear();
}

ImportGrammarSelectFilesPage::~ImportGrammarSelectFilesPage()
{
}
