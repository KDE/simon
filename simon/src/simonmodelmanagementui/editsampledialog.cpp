/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#include "editsampledialog.h"
#include <simonscenarios/trainingmanager.h>
#include <simonsound/recwidget.h>
#include <QWidget>
#include <QFile>
#include <KStandardDirs>
#include <KMessageBox>

EditSampleDialog::EditSampleDialog(QWidget* parent, Qt::WFlags flags): KDialog(parent, flags)
{
  QWidget *w = new QWidget(this);
  ui.setupUi(w);
  setMainWidget(w);
}

bool EditSampleDialog::editSample(const QString& key)
{
  PromptsTable *prompts(TrainingManager::getInstance()->getPrompts());
  
  ui.leSampleGroup->setText(prompts->sampleGroup(key));
  //copy recording to temporary place and link rec widget to that copy
  QString path = TrainingManager::getInstance()->getTrainingDir() + key + ".wav";
  QString tmpPathScheme = KStandardDirs::locateLocal("tmp", "simon/edit-sample");
  QString tmpPath = tmpPathScheme + ".wav";
  if ((QFile::exists(tmpPath) && (!QFile::remove(tmpPath))) ||
      (!QFile::copy(path, tmpPath))) {
    KMessageBox::sorry(this, i18n("Failed to copy sample to temporary path."));
    return false;
  }
  
  QString prompt = prompts->value(key);
  RecWidget *recWidget = new RecWidget(QString(), prompt, tmpPathScheme, true, this);
  dynamic_cast<QVBoxLayout*>(mainWidget()->layout())->insertWidget(1, recWidget);
  
  bool success = true;
  if (exec()) {
    // check if file still exists - if not, ask to remove the original sample
    if (!QFile::exists(tmpPath)) {
      if (KMessageBox::questionYesNo(this, i18n("You have removed the sample.\n\nDo you want to remove the "
                                                "original sample from the training corpus?\n\n(Selecting "
                                                "\"No\" will revert the training corpus back to the state "
                                                "before opening this sample.)")) == KMessageBox::Yes) {
        if (!prompts->deletePrompt(key)) {
          KMessageBox::sorry(this, i18n("Failed to remove the sample from the training corpus."));
          success = false;
        }
      }
    } else {
      int revision = 0;
      QString newKey; 
      while (prompts->contains(newKey = QString::number(revision)+key))
        ++revision;
      
      // if the file still exists, store the (possibly new) sample group and replace the original file with the
      // new one from the temporary location
      if (!prompts->deletePrompt(key) || !TrainingManager::getInstance()->savePrompts()) {
        KMessageBox::sorry(this, i18n("Failed to remove old version of sample."));
        success = false;
      }
      if (!QFile::copy(tmpPath, TrainingManager::getInstance()->getTrainingDir() + newKey + ".wav")) {
        KMessageBox::sorry(this, i18n("Failed to copy new version of sample to target location."));
        success = false;
      } else
        prompts->insert(newKey, ui.leSampleGroup->text(), prompt);
    }
  }
  delete recWidget;
  return success;
}
