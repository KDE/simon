//
// C++ Implementation: importgrammarselectfilespage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
