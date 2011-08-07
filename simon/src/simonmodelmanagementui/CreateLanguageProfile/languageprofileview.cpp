/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#include "languageprofileview.h"
#include "ui_languageprofileview.h"
#include <unistd.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/shadowvocabulary.h>
#include <QWidget>
#include <KProcess>
#include <KStandardDirs>
#include <KMessageBox>
#include <KDebug>
#include <KPushButton>

LanguageProfileView::LanguageProfileView(QWidget* parent, Qt::WFlags flags): KDialog(parent, flags),
ui(new Ui::LanguageProfileView),
sequitur(new KProcess(this)),
state(LanguageProfileView::Idle)
{
  QWidget *widget = new QWidget(this);
  ui->setupUi(widget);
  setMainWidget(widget);
  setWindowTitle(i18n("Create language profile..."));
  
  
  setButtonText(Ok, i18n("Create profile"));
  
  connect(sequitur, SIGNAL(readyReadStandardError()), this, SLOT(parseErrorLog()));
  
  connect(sequitur, SIGNAL(finished(int)), this, SLOT(nextStep()));
}

void LanguageProfileView::slotButtonClicked(int button)
{
  if ((button == Ok) && (state == Idle))
    createProfile();
  else
    KDialog::slotButtonClicked(button);
}


void LanguageProfileView::createProfile()
{
  button(Ok)->setEnabled(false);
  sequiturExe = KStandardDirs::findExe("g2p.py");
  
  if (sequiturExe.isEmpty()) {
    KMessageBox::error(this, i18n("Sequitur G2P is required to use this feature.\n\n"
	"It's free software and you can download it from "
	"<a href=\"http://www-i6.informatik.rwth-aachen.de/web/Software/g2p.html\">"
	"its homepage the university of Aachen</a>."));
    return;
  }
  
  sequitur->setWorkingDirectory(KStandardDirs::locateLocal("tmp", "simon/sequitur/"));
  sequitur->setOutputChannelMode(KProcess::OnlyStderrChannel);
  
  //prepare sphinx dict
  if (!ScenarioManager::getInstance()->getShadowVocabulary()->
    exportToFile(KStandardDirs::locateLocal("tmp", "simon/sequitur/train.lex"), Vocabulary::SPHINX)) {
    KMessageBox::sorry(this, i18n("Couldn't export current shadow dictionary to file for further processing."));
    return;
  }
 
  nextStep();
}

void LanguageProfileView::nextStep()
{
  switch(state) {
    case Idle:
      sequitur->setProgram(sequiturExe, QStringList() << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model1");
      state = Initial;
      ui->pbTotal->setValue(3);
      ui->pbTotal->setFormat("Creating initial model...");
      break;
    case Initial:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model1" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model2");
      state = RampUp1;
      ui->pbTotal->setValue(7);
      ui->pbTotal->setFormat("Improving model (1/4)...");
      break;
    case RampUp1:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model2" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model3");
      state = RampUp2;
      ui->pbTotal->setValue(15);
      ui->pbTotal->setFormat("Improving model (2/4)...");
      break;
    case RampUp2:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model3" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model4");
      state = RampUp3;
      ui->pbTotal->setValue(32);
      ui->pbTotal->setFormat("Improving model (3/4)...");
      break;
    case RampUp3:
      sequitur->setProgram(sequiturExe, QStringList() << "--model" << "model4" << "--ramp-up" << "--train" << "train.lex" << "--devel" << "5%" << "--write-model" << "model5");
      state = RampUp4;
      ui->pbTotal->setValue(58);
      ui->pbTotal->setFormat("Improving model (4/4)...");
      break;
    case RampUp4:
      ui->pbTotal->setValue(100);
      ui->pbTotal->setFormat("Finished.");
      setButtonText(Ok, i18n("Ok"));
      button(Ok)->setEnabled(true);
      break;
    case Error:
      button(Ok)->setEnabled(true);
      return;
    default:
      kDebug() << "Not implemented";
      return;
  };

   sequitur->start();
}

void LanguageProfileView::parseErrorLog()
{
  disconnect(sequitur, SIGNAL(readyReadStandardError()), this, SLOT(parseErrorLog()));
  
  QString error = QString::fromUtf8(sequitur->readAllStandardError());
  KMessageBox::detailedError(this, i18n("An unexpected error occurred when creating the model with sequitur."), error);
  
  sequitur->terminate(); //if we are not yet done, abort
  state = Error;
  kDebug() << "Error: " << error;
  
  connect(sequitur, SIGNAL(readyReadStandardError()), this, SLOT(parseErrorLog()));
}

void LanguageProfileView::done(int p)
{
  disconnect(sequitur);
  sequitur->terminate();
  QDialog::done(p);
}

