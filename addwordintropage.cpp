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

/**
 * \brief Constructor - also creates the GUI Elements
 * \author Peter Grasch
 * @param parent
 * The parent of the widget
 */
AddWordIntroPage::AddWordIntroPage(QWidget* parent): QWizardPage(parent)
{
	
	setTitle(tr("Hinzufügen eines neuen Wortes"));
	QLabel *label = new QLabel(this);
	label->setText(tr("Mit Hilfe dieses Assistenten koennen Sie neue Wörter zum\nSprachmodell hinzufügen. Geben Sie hierzu\nbitte den Namen des Wortes an\nund bestätigen Sie mit \"Weiter\":\n\n"));
	QLabel *lbName = new QLabel(this);
	lbName->setText(tr("Neues Wort:"));
	leName = new QLineEdit(this);
	QVBoxLayout *layout = new QVBoxLayout(this);
	QHBoxLayout *loName = new QHBoxLayout();
	loName->addWidget(lbName);
	loName->addWidget(leName);

	layout->addWidget(label);
	registerField(tr("name*"), leName);
	layout->addLayout(loName);
	
	setLayout(layout);
}

/**
 * \brief writes a word in the lineedit
 * \author Susanne Tschernegg
 */
void AddWordIntroPage::setName(QString word)
{
    leName->setText(word);
}


