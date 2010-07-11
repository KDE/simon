/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "firstrunwizard.h"
#include "firstrunsimondconfig.h"
#include "firstrunscenariosconfig.h"
#include "firstrunsoundconfig.h"
#include "firstrunbasemodelconfig.h"
#include <simonsound/trainsamplevolumepage.h>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>

FirstRunWizard::FirstRunWizard(QWidget* parent, Qt::WFlags flags)
: SimonWizard(parent, flags)
{
  addPage(createIntroPage());
  addPage(createScenariosConfigPage());
  addPage(createBaseModelConfigPage());
  addPage(createSimondConfigPage());
  addPage(createSoundConfigPage());
  addPage(createSoundVolumePage());
  addPage(createFinishedPage());
  setBanner("firstrun");
}


QWizardPage* FirstRunWizard::createIntroPage()
{
  QWizardPage *intro = new QWizardPage(this);
  QHBoxLayout *lay = new QHBoxLayout(intro);
  QLabel *desc = new QLabel(intro);
  lay->addWidget(desc);
  intro->setLayout(lay);

  desc->setWordWrap(true);
  intro->setTitle(i18n("Welcome to simon"));
  desc->setOpenExternalLinks(true);
  desc->setText(i18n("<html><head /><body><p>simon is an open source speech recognition solution enabling you to control your computer with voice commands.</p><p>This assistant will guide you through some key settings that will help you to get simon up and running.</p>"
    "<p><em>Please</em> follow the following steps very carefully and read the accompanying instructions.</p><p>You can change all these options later on manually too (see the manual for more infromation). You can find an option to display the wizard on the next start in the simon configuration window.</p><p>You can find some beginners instructions on <a href=\"http://simon-listens.org/wiki\">our wiki</a>.</p></body></html>"));

  return intro;
}


QWizardPage* FirstRunWizard::createSimondConfigPage()
{
  return new FirstRunSimondConfig(this);
}


QWizardPage* FirstRunWizard::createSoundConfigPage()
{
  return new FirstRunSoundConfig(this);
}


QWizardPage* FirstRunWizard::createSoundVolumePage()
{
  return new TrainSampleVolumePage(this);
}


QWizardPage* FirstRunWizard::createBaseModelConfigPage()
{
  return new FirstRunBaseModelConfig(this);
}


QWizardPage* FirstRunWizard::createScenariosConfigPage()
{
  return new FirstRunScenariosConfig(this);
}


QWizardPage* FirstRunWizard::createFinishedPage()
{
  QWizardPage *finish = new QWizardPage(this);
  QHBoxLayout *lay = new QHBoxLayout(finish);
  QLabel *desc = new QLabel(finish);
  lay->addWidget(desc);
  finish->setLayout(lay);

  desc->setWordWrap(true);
  finish->setTitle(i18n("Finished"));
  desc->setText(i18n("<html><head /><body><p>You completed the initial setup of simon.</p><p>You can find more information on simon and helpful guides and instructions in the simon manual (press F1 in the simon main window to display).</p></body></html>"));

  return finish;
}


FirstRunWizard::~FirstRunWizard()
{
}
