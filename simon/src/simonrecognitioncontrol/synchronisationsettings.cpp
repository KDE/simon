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

#include "synchronisationsettings.h"
#include "recognitionconfiguration.h"
#include "recognitioncontrol.h"

#include <QListWidgetItem>

#include <KIcon>
#include <KMessageBox>
#include <KProgressDialog>

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
SynchronisationSettings::SynchronisationSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
  Q_UNUSED(args);

  dlg=0;
  ui.setupUi(this);

  ui.pbLoadList->setIcon(KIcon("view-refresh"));
  ui.pbSelectModel->setIcon(KIcon("dialog-ok-apply"));

  addConfig(RecognitionConfiguration::self(), this);

  connect(ui.pbLoadList, SIGNAL(clicked()), this, SLOT(loadList()));
  connect(ui.pbSelectModel, SIGNAL(clicked()), this, SLOT(selectModel()));
  connect(ui.lwModels, SIGNAL(currentRowChanged(int)), this, SLOT(modelSelectionChanged()));
  connect(ui.lwModels, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(selectModel()));
  connect(RecognitionControl::getInstance(), SIGNAL(modelsAvailable(QList<QDateTime>)), this, SLOT(displayList(QList<QDateTime>)));
  connect(RecognitionControl::getInstance(), SIGNAL(loggedIn()), this, SLOT(connected()));
  connect(RecognitionControl::getInstance(), SIGNAL(disconnected()), this, SLOT(disconnected()));
}


void SynchronisationSettings::connected()
{
  loadList();
}


void SynchronisationSettings::disconnected()
{
  ui.lwModels->clear();
  ui.pbSelectModel->setEnabled(false);
  ui.lwModels->setEnabled(false);
}


void SynchronisationSettings::loadList()
{
  ui.lwModels->clear();

  if (!RecognitionControl::getInstance()->isConnected()) {
    KMessageBox::information(this, i18n("Not connected to the server"));
    ui.pbSelectModel->setEnabled(false);
    ui.lwModels->setEnabled(false);
    return;
  }

  if (!dlg) {
    dlg = new KProgressDialog(this, i18n("Loading available Models"), i18n("Loading list of available Models"));
    dlg->progressBar()->setValue(0);
    dlg->progressBar()->setMaximum(0);
    dlg->showCancelButton(false);
  }
  else dlg->show();

  if (!RecognitionControl::getInstance()->getAvailableModels()) {
    KMessageBox::sorry(this, i18n("Could not send Request to Server"));
    dlg->reject();
    dlg->deleteLater();
    dlg=0;
  }
}


void SynchronisationSettings::displayList(const QList<QDateTime>& models)
{
  ui.lwModels->clear();
  if (dlg) {
    dlg->progressBar()->setValue(1);
    dlg->progressBar()->setMaximum(1);
    dlg->accept();
    dlg->deleteLater();
    dlg=0;
  }
  if (models.isEmpty() && isVisible()) {
    KMessageBox::sorry(this, i18n("No Models found"));
    return;
  }

  foreach (const QDateTime& date, models) {
    QListWidgetItem *item = new QListWidgetItem(ui.lwModels);
    item->setText(date.toString());
    item->setData(Qt::UserRole, date);
    ui.lwModels->addItem(item);
  }

  ui.lwModels->setCurrentRow(models.count() -1);
  ui.pbSelectModel->setEnabled(false);            //current model is selected
  ui.lwModels->setEnabled(models.count() > 1);
}


void SynchronisationSettings::modelSelectionChanged()
{
  if (ui.lwModels->currentRow() == ui.lwModels->count()-1)
    //this is either -1 or the already active model
    ui.pbSelectModel->setEnabled(false);
  else ui.pbSelectModel->setEnabled(true);
}


void SynchronisationSettings::showEvent(QShowEvent*)
{
  if (RecognitionControl::getInstance()->isConnected())
    loadList();
}


void SynchronisationSettings::selectModel()
{
  if (ui.lwModels->currentRow() == -1) {
    KMessageBox::information(this, i18n("Please select a Model from the List"));
    return;
  }

  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to revert all changes made since this Model?")) != KMessageBox::Yes)
    return;

  QDateTime modelDate = ui.lwModels->currentItem()->data(Qt::UserRole).toDateTime();
  if (modelDate.isNull()) {
    KMessageBox::sorry(this, i18n("This model has no valid date."));
    return;
  }

  if (RecognitionControl::getInstance()->switchToModel(modelDate))
    KMessageBox::information(this, i18n("The Request has been send to the server.\n\nPlease track the process of the "
      "synchronization in simons main window."));
  else
    KMessageBox::sorry(this, i18n("Could not send Request to Server"));
}


SynchronisationSettings::~SynchronisationSettings()
{}
