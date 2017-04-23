/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include <KLocalizedString>
#include <QDebug>

/**
 * \brief Constructor - Inits the gui
 * \author Peter Grasch
 * @param parent Parent of the WizardPage
 */
ImportDictSelectSourcePage::ImportDictSelectSourcePage(QWidget* parent): QWizardPage(parent)
{
  setTitle(i18n("Select the Type of Dictionary"));
  ui.setupUi(this);

  registerField("hadifix", ui.rbHadifixBOMP, "checked", SIGNAL(toggled(bool)));
  registerField("lexicon", ui.rbHTK, "checked", SIGNAL(toggled(bool)));
  registerField("pls", ui.rbPLS, "checked", SIGNAL(toggled(bool)));
  registerField("sphinx", ui.rbSPHINX, "checked", SIGNAL(toggled(bool)));
  registerField("julius", ui.rbJuliusVocabulary, "checked", SIGNAL(toggled(bool)));
}


/**
 * \author Peter Grasch
 * @return the id
 */
int ImportDictSelectSourcePage::nextId() const
{
  if (field("hadifix").toBool())
    return ImportDictView::BompPage;
  else if (field("lexicon").toBool())
    return ImportDictView::LexiconPage;
  else if (field("pls").toBool())
    return ImportDictView::PLSPage;
  else if (field("julius").toBool()) {
    qDebug() << "julius selected";
    return ImportDictView::JuliusVocabulary;
  } else
  return ImportDictView::SPHINXPage;
}


/**
 * \brief destructor
 * \author Peter Grasch
 */
ImportDictSelectSourcePage::~ImportDictSelectSourcePage()
{
}
