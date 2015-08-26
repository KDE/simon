/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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
#include <QLabel>
#include <KI18n/klocalizedstring.h>

FirstRunWizard::FirstRunWizard(QWidget* parent, Qt::WFlags flags)
: SimonWizard(parent, flags)
{
  addPage(createIntroPage());
  addPage(createScenariosConfigPage());
  addPage(createBaseModelConfigPage());
  addPage(createSimondConfigPage());
  addPage(createSoundConfigPage());
  addPage(createSoundVolumePage());
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
  intro->setTitle(i18n("Welcome to Simon"));
  desc->setOpenExternalLinks(true);
  desc->setText(i18n("<html><head /><body>"
    "<p>Simon is a speech recognition solution enabling you to control your computer with your voice.</p>"
    "<p>This assistant will help you to get Simon up and running.</p>"
    "<p>Go through the following steps carefully and read the instructions.</p>"
    "<p>You can still change these options later (see the manual for more information)."
    "<p>Find more instructions on <a href=\"http://userbase.kde.org/Simon\">our wiki</a>.</p>"
    "</body></html>"));

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


FirstRunWizard::~FirstRunWizard()
{
}
