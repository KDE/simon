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

#include "mergeterminalsworkingpage.h"
#include "mergeterminals.h"

MergeTerminalsWorkingPage::MergeTerminalsWorkingPage(QWidget* parent) : QWizardPage(parent),
mergeTerminals(new MergeTerminals(this))
{
  ui.setupUi ( this );

  connect ( mergeTerminals, SIGNAL ( status ( QString ) ), this, SLOT ( displayStatus ( QString ) ) );
  connect ( mergeTerminals, SIGNAL ( done() ), this, SLOT ( finished() ) );
  connect ( mergeTerminals, SIGNAL ( progress ( int, int ) ), this, SLOT ( displayProgress ( int, int ) ) );

  setTitle ( i18n ( "Merging..." ) );
}


void MergeTerminalsWorkingPage::finished()
{
  this->complete = true;
  emit completeChanged();
  emit done();
}


void MergeTerminalsWorkingPage::initializePage()
{
  complete = false;
  mergeTerminals->setTerminalA ( field ( "terminalA" ).toString() );
  mergeTerminals->setTerminalB ( field ( "terminalB" ).toString() );
  mergeTerminals->setNewName ( field ( "newName" ).toString() );
  mergeTerminals->setIncludeShadow ( field ( "includeShadow" ).toBool() );
  mergeTerminals->setIncludeGrammar ( field ( "includeGrammar" ).toBool() );
  mergeTerminals->start();
}


void MergeTerminalsWorkingPage::displayProgress ( int progress, int max )
{
  ui.pbProgress->setMaximum ( max );
  ui.pbProgress->setValue ( progress );
  QCoreApplication::processEvents();
}


void MergeTerminalsWorkingPage::displayStatus ( QString status )
{
  ui.lbStatus->setText ( status );
  QCoreApplication::processEvents();
}


MergeTerminalsWorkingPage::~MergeTerminalsWorkingPage()
{
  mergeTerminals->deleteLater();
}
