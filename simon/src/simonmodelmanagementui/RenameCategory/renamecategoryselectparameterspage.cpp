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

#include "renamecategoryselectparameterspage.h"
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/speechmodel.h>

RenameCategorySelectParametersPage::RenameCategorySelectParametersPage(QWidget *parent)
: QWizardPage(parent)
{
  ui.setupUi(this);
  registerField("renameNewName*",ui.leNewName);
  registerField("renameCategory*",ui.lwCategory, "currentText", SIGNAL(currentRowChanged(int)));
  registerField("renameIncludeShadow", ui.cbIncludeShadow);
  registerField("renameIncludeGrammar", ui.cbIncludeGrammar);
}


void RenameCategorySelectParametersPage::initializePage()
{
  QStringList availableCategories;
  availableCategories = ScenarioManager::getInstance()->getCategories((SpeechModel::ModelElements)
    (SpeechModel::ShadowVocabulary|SpeechModel::ScenarioVocabulary|SpeechModel::ScenarioGrammar));
  ui.lwCategory->clear();
  ui.lwCategory->addItems(availableCategories);
}


RenameCategorySelectParametersPage::~RenameCategorySelectParametersPage()
{
}
