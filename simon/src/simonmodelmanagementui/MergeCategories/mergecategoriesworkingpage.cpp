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

#include "mergecategoriesworkingpage.h"
#include "mergecategories.h"

MergeCategoriesWorkingPage::MergeCategoriesWorkingPage(QWidget* parent) : QWizardPage(parent),
mergeCategories(new MergeCategories(this))
{
  ui.setupUi ( this );

  connect ( mergeCategories, SIGNAL (status(QString)), this, SLOT (displayStatus(QString)) );
  connect ( mergeCategories, SIGNAL (done()), this, SLOT (finished()) );
  connect ( mergeCategories, SIGNAL (progress(int,int)), this, SLOT (displayProgress(int,int)) );

  setTitle ( i18n ( "Merging..." ) );
}


void MergeCategoriesWorkingPage::finished()
{
  this->complete = true;
  emit completeChanged();
  emit done();
}


void MergeCategoriesWorkingPage::initializePage()
{
  complete = false;
  mergeCategories->setCategoryA ( field ( "categoryA" ).toString() );
  mergeCategories->setCategoryB ( field ( "categoryB" ).toString() );
  mergeCategories->setNewName ( field ( "newName" ).toString() );
  mergeCategories->setIncludeShadow ( field ( "includeShadow" ).toBool() );
  mergeCategories->setIncludeGrammar ( field ( "includeGrammar" ).toBool() );
  mergeCategories->start();
}


void MergeCategoriesWorkingPage::displayProgress ( int progress, int max )
{
  ui.pbProgress->setMaximum ( max );
  ui.pbProgress->setValue ( progress );
  QCoreApplication::processEvents();
}


void MergeCategoriesWorkingPage::displayStatus ( QString status )
{
  ui.lbStatus->setText ( status );
  QCoreApplication::processEvents();
}


MergeCategoriesWorkingPage::~MergeCategoriesWorkingPage()
{
  mergeCategories->deleteLater();
}
