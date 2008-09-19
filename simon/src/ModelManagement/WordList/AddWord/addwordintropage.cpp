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
#include <KLineEdit>
#include <KLocalizedString>

/**
 * \brief Constructor - also creates the GUI Elements
 * \author Peter Grasch
 * @param parent
 * The parent of the widget
 */
AddWordIntroPage::AddWordIntroPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Hinzufügen eines neuen Wortes"));
	QLabel *label = new QLabel(this);
	label->setWordWrap(true);
	label->setText(i18n("Mit Hilfe dieses Assistenten können Sie neue Wörter zum Sprachmodell hinzufügen.\n\nGeben Sie hierzu bitte den Namen des Wortes an und bestätigen Sie mit \"Weiter\":\n\n"));
	QLabel *lbName = new QLabel(this);
	lbName->setText(i18n("Neues Wort:"));
	KLineEdit *leName = new KLineEdit(this);
	QVBoxLayout *layout = new QVBoxLayout(this);
	QHBoxLayout *loName = new QHBoxLayout();
	loName->addWidget(lbName);
	loName->addWidget(leName);

	layout->addWidget(label);
	registerField("wordNameIntro*", leName);
	layout->addLayout(loName);
	
	setLayout(layout);
}

