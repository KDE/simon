//
// C++ Implementation: importdictselectsourcepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importdictselectsourcepage.h"
#include "importdictview.h"
#include <QLabel>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVariant>
#include <KLocalizedString>

/**
 * \brief Constructor - Inits the gui
 * \author Peter Grasch
 * @param parent Parent of the WizardPage
 */
ImportDictSelectSourcePage::ImportDictSelectSourcePage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Wörterbuch Typ auswählen"));
	QVBoxLayout *lay = new QVBoxLayout(this);
	
	QLabel *desc = new QLabel(this);
	desc->setText(i18n("Simon kann drei verschiedene Typen von Wörterbüchern importieren: HADIFIX-BOMP Wörterbücher, Wiktionary-Dumps und HTK Wörterbücher.\n\nBitte selektieren Sie welchen Typ von Wörterbuch Sie importieren möchten:\n"));
	desc->setWordWrap(true);
	
	QRadioButton *hadifixBOMP=new QRadioButton(i18n("HADIFIX-BOMP"), this);
	QRadioButton *wiktionary = new QRadioButton(i18n("Wiktionary Wörterbuch"), this);
	QRadioButton *lexicon = new QRadioButton(i18n("simon kompatibles Lexicon"), this);
	none = new QRadioButton(i18n("Keines (Überspringt diesen Schritt)"), this);
	
	registerField("hadifix", hadifixBOMP, "checked", SIGNAL(toggled(bool)));
	registerField("wiktionary", wiktionary, "checked", SIGNAL(toggled(bool)));
	registerField("lexicon", lexicon, "checked", SIGNAL(toggled(bool)));
	registerField("NoDictionary", none, "checked", SIGNAL(toggled(bool)));

	lay->addWidget(desc);
	lay->addWidget(hadifixBOMP);
	lay->addWidget(wiktionary);
	lay->addWidget(lexicon);
	lay->addWidget(none);

	none->hide();
	setLayout(lay);

	hadifixBOMP->setChecked(true);
}
/**
 * \brief Returns the next id for the wizard (2 if we selected hadifix, else 3)
 * \author Peter Grasch
 * @return the id
 */
int ImportDictSelectSourcePage::nextId() const
{
	if (field("hadifix").toBool())
		return ImportDictView::BompPage;

	if (field("wiktionary").toBool())
		return ImportDictView::WiktionaryPage;

	if (field("lexicon").toBool())
		return ImportDictView::LexiconPage;

	return ImportDictView::FinishedPage;
}

/**
 * \brief destructor
 * \author Peter Grasch
 */
ImportDictSelectSourcePage::~ImportDictSelectSourcePage()
{
}


