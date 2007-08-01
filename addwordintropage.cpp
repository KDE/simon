//
// C++ Implementation: addwordintropage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "addwordintropage.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

AddWordIntroPage::AddWordIntroPage(QWidget* parent): QWizardPage(parent)
{
	
	setTitle(tr("Hinzufuegen eines neuen Wortes"));
	QLabel *label = new QLabel(this);
	label->setText("Mit Hilfe dieses Assistenten koennen Sie neue Woerter zum\nSprachmodell hinzufuegen. Geben Sie hierzu\nbitte den Namen des Wortes an\nund bestaetigen Sie mit \"Weiter\":\n\n");
	QLabel *lbName = new QLabel(this);
	lbName->setText(tr("Neues Wort:"));
	QLineEdit *leName = new QLineEdit(this);
	QVBoxLayout *layout = new QVBoxLayout(this);
	QHBoxLayout *loName = new QHBoxLayout(this);
	loName->addWidget(lbName);
	loName->addWidget(leName);

	layout->addWidget(label);
	registerField("name*", leName);
	layout->addLayout(loName);
	
	setLayout(layout);
}


AddWordIntroPage::~AddWordIntroPage()
{
}


