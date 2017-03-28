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

#include "editworddialog.h"
#include <simonscenarios/word.h>
#include <simonscenarios/scenariomanager.h>
#include <KInputDialog>
#include <KIcon>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the dialog
 * @param f Window flags
 */
EditWordDialog::EditWordDialog(QWidget* parent, Qt::WindowFlags f)
  : KDialog(parent, f)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);
  setMainWidget( widget );
  setCaption( i18n("Edit Word") );
  ui.tbAddCategory->setIcon(KIcon("list-add"));
  connect(ui.tbAddCategory, SIGNAL(clicked()), this, SLOT(addCategory()));
}


/**
 * \brief Executes the dialog
 * @param word The word to edit
 * @return False, if the dialog is aborted
 */
int EditWordDialog::exec(Word *word)
{
  ui.leWord->setText(word->getWord());
  ui.cbType->clear();
  QStringList categories = ScenarioManager::getInstance()->getCategories(
    (SpeechModel::ModelElements)
    (SpeechModel::ShadowVocabulary|
    SpeechModel::ScenarioVocabulary|
    SpeechModel::AllScenariosVocabulary|
    SpeechModel::AllScenariosGrammar|
    SpeechModel::ScenarioGrammar));
  if (!categories.contains(i18nc("Standard category for unused words", "Unused")))
    categories << i18nc("Standard category for unused words", "Unused");
  ui.cbType->addItems(categories);
  ui.cbType->setCurrentIndex(ui.cbType->findText(word->getCategory()));
  ui.leSampa->setText(word->getPronunciation());

  int ret = KDialog::exec();

  if (ret) {
    //update word
    word->setWord(ui.leWord->text());
    word->setCategory(ui.cbType->currentText());
    word->setPronunciation(ui.leSampa->text());
  }

  return ret;
}


/**
 * \brief Queries the User for the new name and adds the category to the list
 * \author Peter Grasch
 */
void EditWordDialog::addCategory()
{
  QString newCategory = KInputDialog::getText(i18n("Add Category"), i18n("Name of new category:"));

  if (newCategory.isEmpty()) return;

  ui.cbType->addItem(newCategory);
  ui.cbType->setCurrentIndex(ui.cbType->count()-1);
}
