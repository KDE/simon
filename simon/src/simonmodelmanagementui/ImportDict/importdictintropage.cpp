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

#include "importdictintropage.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <KI18n/klocalizedstring.h>

/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportDictIntroPage::ImportDictIntroPage(QWidget* parent): QWizardPage(parent)
{
  setTitle(i18n("Import Dictionary"));

  QLabel *lbIntro = new QLabel(this);
  lbIntro->setWordWrap(true);
  lbIntro->setOpenExternalLinks(true);
  lbIntro->setText(i18n("<html><head /><body>"
    "<p>This assistant will help you to import a new dictionary.</p><p>A dictionary "
    "contains information about the known words like how they are written and how "
    "they are pronounced.</p><p>The dictionary is an essential core component of your "
    "language model. Please ensure that every dictionary that you import is of "
    "high quality as it will massively impact your recognition performance.</p><p>We "
    "suggest that you use the Voxforge English Dictionary which is a HTK "
    "compatible lexicon and of very high quality.</p><p>"
    "For more information about available dictionaries, please <a "
    "href=\"http://userbase.kde.org/Simon/Shadow_dictionary\">"
    "visit our Wiki</a>.</body></html>"));

  QLabel *lbTarget = new QLabel(this);
  lbTarget->setText(i18n("Target:"));

  QComboBox *cbTarget = new QComboBox(this);
  cbTarget->setToolTip(i18n("Defines where the new words will be added to."));
  cbTarget->addItem(i18n("Shadow Dictionary"));
  cbTarget->addItem(i18n("Active Dictionary"));
  registerField("targetType", cbTarget, "currentIndex", SIGNAL(currentIndexChanged(int)));

  QHBoxLayout *targetLay = new QHBoxLayout();
  targetLay->addWidget(lbTarget);
  targetLay->addWidget(cbTarget);

  QVBoxLayout *lay = new QVBoxLayout(this);
  lay->addWidget(lbIntro);
  lay->addLayout(targetLay);
  setLayout(lay);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictIntroPage::~ImportDictIntroPage()
{
}
