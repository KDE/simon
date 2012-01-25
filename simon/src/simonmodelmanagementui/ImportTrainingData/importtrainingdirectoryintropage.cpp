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

#include "importtrainingdirectoryintropage.h"
#include <QDir>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
ImportTrainingDirectoryIntroPage::ImportTrainingDirectoryIntroPage(QWidget *parent) : QWizardPage(parent)
{
  ui.setupUi(this);

  ui.urPrompts->setMode(KFile::File | KFile::ExistingOnly | KFile::LocalOnly);
  ui.urBaseDirectory->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
  ui.urTrainingDataDirectory->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);

  setTitle(i18n("Import Training Samples"));

  connect(ui.urTrainingDataDirectory, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.urPrompts, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.rbPrompts, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
  connect(ui.urBaseDirectory, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.urPrompts, SIGNAL(textChanged(QString)), this, SLOT(promptsPathChanged()));

  registerField("importPrompts", ui.rbPrompts);
  registerField("directory", ui.urTrainingDataDirectory, "url", SIGNAL(textChanged(QString)));
  registerField("prompts", ui.urPrompts, "url", SIGNAL(textChanged(QString)));
  registerField("promptsBaseDirectory", ui.urBaseDirectory, "url", SIGNAL(textChanged(QString)));

  connect(ui.rbPrompts, SIGNAL(toggled(bool)), ui.lbPrompts, SLOT(setVisible(bool)));
  connect(ui.rbPrompts, SIGNAL(toggled(bool)), ui.urPrompts, SLOT(setVisible(bool)));
  connect(ui.rbPrompts, SIGNAL(toggled(bool)), ui.lbBaseDirectory, SLOT(setVisible(bool)));
  connect(ui.rbPrompts, SIGNAL(toggled(bool)), ui.urBaseDirectory, SLOT(setVisible(bool)));
  connect(ui.rbFolder, SIGNAL(toggled(bool)), ui.lbFolder, SLOT(setVisible(bool)));
  connect(ui.rbFolder, SIGNAL(toggled(bool)), ui.urTrainingDataDirectory, SLOT(setVisible(bool)));

  ui.lbFolder->hide();
  ui.urTrainingDataDirectory->hide();
}


void ImportTrainingDirectoryIntroPage::promptsPathChanged()
{
  KUrl url = ui.urPrompts->url();
}


bool ImportTrainingDirectoryIntroPage::isComplete() const
{
  if (ui.rbPrompts->isChecked()) {
    QString dir = ui.urBaseDirectory->url().path();
    return QFile::exists(ui.urPrompts->url().path()) &&
      ((!dir.isEmpty()) && QDir(dir).exists());
  }
  else {
    QString dir = ui.urTrainingDataDirectory->url().path();
    return ((!dir.isEmpty()) && QDir(dir).exists());
  }
}
