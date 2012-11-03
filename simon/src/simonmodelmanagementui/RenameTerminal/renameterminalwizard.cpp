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

#include "renameterminalwizard.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <KStandardDirs>
#include "renameterminalselectparameterspage.h"
#include "renameterminalworkingpage.h"

RenameTerminalWizard::RenameTerminalWizard(QWidget* parent): SimonWizard(parent)
{
  setWindowTitle(i18n("Rename terminal"));
  addPage(createIntroPage());
  addPage(createSelectParametersPage());
  addPage(createWorkingPage());
  addPage(createFinishedPage());
  setBanner("editterminal");
}


QWizardPage* RenameTerminalWizard::createSelectParametersPage()
{
  return  new RenameTerminalSelectParametersPage(this);
}


QWizardPage* RenameTerminalWizard::createWorkingPage()
{
  RenameTerminalWorkingPage *work =  new RenameTerminalWorkingPage(this);
  connect(work, SIGNAL(done()), this, SLOT(next()));
  return work;
}


QWizardPage* RenameTerminalWizard::createIntroPage()
{
  QWizardPage *intro = new QWizardPage(this);
  QHBoxLayout *lay = new QHBoxLayout(intro);
  QLabel *desc = new QLabel(intro);
  lay->addWidget(desc);
  intro->setLayout(lay);

  desc->setWordWrap(true);
  intro->setTitle(i18n("Welcome to the renaming of a terminal"));
  desc->setText(i18n("This assistant will allow you to rename an existing terminal. "));

  return intro;
}


QWizardPage* RenameTerminalWizard::createFinishedPage()
{
  QWizardPage *finished = new QWizardPage(this);
  QHBoxLayout *lay = new QHBoxLayout(finished);
  QLabel *desc = new QLabel(finished);
  lay->addWidget(desc);
  finished->setLayout(lay);

  desc->setWordWrap(true);
  finished->setTitle(i18n("Renaming complete"));
  desc->setText(i18n("The terminal has been renamed.\n\nThank you for improving Simon."));

  return finished;
}


RenameTerminalWizard::~RenameTerminalWizard()
{
}
