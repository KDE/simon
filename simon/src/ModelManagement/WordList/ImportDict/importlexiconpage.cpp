//
// C++ Implementation: importlexiconpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importlexiconpage.h"

ImportLexiconPage::ImportLexiconPage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle(i18n("Lexikon auswälen"));
	registerField("lexiconFilename", ui.urPath, "url", SIGNAL(textChanged(QString)));
}


ImportLexiconPage::~ImportLexiconPage()
{
}


