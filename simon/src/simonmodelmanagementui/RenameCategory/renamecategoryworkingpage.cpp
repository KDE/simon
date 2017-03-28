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

#include "renamecategoryworkingpage.h"
#include "renamecategory.h"

RenameCategoryWorkingPage::RenameCategoryWorkingPage(QWidget* parent)
: QWizardPage(parent),
complete(false),
renameCategory(new RenameCategory(this))
{
  connect(renameCategory, SIGNAL(progress(int)), this, SLOT(displayProgress(int)));
  connect(renameCategory, SIGNAL(done()), this, SLOT(finish()));
  ui.setupUi(this);
  setTitle(i18n("Renaming category..."));
}


void RenameCategoryWorkingPage::displayProgress(int progress)
{
  ui.pgProgress->setValue(progress);
}


void RenameCategoryWorkingPage::finish()
{
  complete = true;
  emit completeChanged();
  emit done();
}


void RenameCategoryWorkingPage::initializePage()
{
  //do the work
  renameCategory->setOldName(field("renameCategory").toString());

  renameCategory->setNewName(field("renameNewName").toString());
  renameCategory->setIncludeShadow((field("renameIncludeShadow").toBool()));
  renameCategory->setIncludeGrammar(field("renameIncludeGrammar").toBool());
  renameCategory->start();
}


RenameCategoryWorkingPage::~RenameCategoryWorkingPage()
{
}
