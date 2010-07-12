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

#include "addwordintropage.h"
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
  setTitle(i18n("Add a new Word"));

  ui.setupUi(this);

  registerField("wordNameIntro*", ui.leName);
  setCommitPage(true);
  setButtonText(QWizard::CommitButton, i18n("Next"));
}


bool AddWordIntroPage::validatePage()
{
  //kDebug() << "Setting field from: " << field("wordNameIntro");
  QStringList words = field("wordNameIntro").toString().replace(',', ' ').replace('.', ' ').split(' ', QString::SkipEmptyParts);
  setField("wordNameIntro", words.join(" "));
  //kDebug() << "To: " << field("wordNameIntro");
  return true;
}
