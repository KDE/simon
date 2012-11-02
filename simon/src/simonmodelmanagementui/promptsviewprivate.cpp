/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#include "promptsviewprivate.h"
#include "ImportTrainingData/importtrainingdirectory.h"
#include "editsampledialog.h"
#include <simonscenarios/trainingmanager.h>
#include <QWidget>
#include <KMessageBox>

PromptsViewPrivate::PromptsViewPrivate(QWidget *parent)
: QWidget(parent)
{
  ui.setupUi(this);
  ui.tvSamples->setModel(&m_proxy);

  m_proxy.setSourceModel(TrainingManager::getInstance()->getPrompts()->getModel());
  m_proxy.setFilterKeyColumn(0);
  m_proxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
  connect(ui.leSampleSearch, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));
  ui.tvSamples->resizeColumnsToContents();

  connect(ui.pbImportDir, SIGNAL(clicked()), this, SLOT(importDirectory()));
  connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(clearTrainingData()));
  connect(ui.pbOpen, SIGNAL(clicked()), this, SLOT(openSample()));

  ui.pbClear->setIcon(KIcon("edit-clear-list"));
  ui.pbOpen->setIcon(KIcon("document-edit"));
  ui.pbImportDir->setIcon(KIcon("document-open-folder"));
}

void PromptsViewPrivate::filter(const QString& str)
{
  m_proxy.setFilterFixedString(str);
}

void PromptsViewPrivate::clearTrainingData()
{
  if (KMessageBox::questionYesNo(this, i18n("Do you really want to clear all the collected samples?")) == KMessageBox::Yes) {
    if (KMessageBox::warningContinueCancel(this, i18n("This will remove every single recording from the training corpus.\n\nAre you absolutely sure you want to continue?")) == KMessageBox::Continue) {
      if (!TrainingManager::getInstance()->clear())
        KMessageBox::information(this, i18n("Could not clear training data"));
    }
  }
}

QString PromptsViewPrivate::getCurrentSample() const
{
  QModelIndex currentIdx = ui.tvSamples->currentIndex();
  if (!currentIdx.isValid()) {
    kDebug() << "What index?";
    return QString();
  }
  int originalIdx = m_proxy.mapToSource(currentIdx).row();
  kDebug() << "Original index: " << originalIdx;
  return TrainingManager::getInstance()->getPrompts()->sample(originalIdx);
}

void PromptsViewPrivate::openSample()
{
  QString sample = getCurrentSample();
  if (sample.isEmpty()) {
    KMessageBox::information(this, i18n("Please select a sample from the list."));
    return;
  }
  QPointer<EditSampleDialog> edit(new EditSampleDialog(this));
  if (!edit->editSample(sample)) {
    KMessageBox::sorry(this, i18n("Failed to modify sample."));
  }
}

void PromptsViewPrivate::importDirectory()
{
  QPointer<ImportTrainingDirectory> importDir(new ImportTrainingDirectory(this));
  importDir->exec();
  delete importDir;
}

