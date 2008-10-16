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


