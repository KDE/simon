//
// C++ Implementation: firstrunimportgrammardescriptionpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "firstrunimportgrammardescriptionpage.h"
#include "firstrunwizard.h"

FirstRunImportGrammarDescriptionPage::FirstRunImportGrammarDescriptionPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle(tr("Grammatik"));
}

int FirstRunImportGrammarDescriptionPage::nextId() const
{
	if (ui.cbSkip->isChecked())
		return FirstRunWizard::SoundSettingsPage;
	else return FirstRunWizard::GrammarSelectFilesPage;
}


FirstRunImportGrammarDescriptionPage::~FirstRunImportGrammarDescriptionPage()
{
}


