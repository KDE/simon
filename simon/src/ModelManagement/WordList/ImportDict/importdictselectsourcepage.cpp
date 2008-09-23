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
#include <QRadioButton>
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
	ui.setupUi(this);
	
	registerField("hadifix", ui.rbHadifixBOMP, "checked", SIGNAL(toggled(bool)));
	registerField("wiktionary", ui.rbWiktionary, "checked", SIGNAL(toggled(bool)));
	registerField("lexicon", ui.rbHTK, "checked", SIGNAL(toggled(bool)));
	registerField("NoDictionary", ui.rbNone, "checked", SIGNAL(toggled(bool)));

	ui.rbNone->hide();
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


