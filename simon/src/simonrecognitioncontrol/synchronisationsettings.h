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

#ifndef SIMON_SYNCHRONISATIONSETTINGS_H_9CAEA39ADADC44EEB28647C537C2BEB3
#define SIMON_SYNCHRONISATIONSETTINGS_H_9CAEA39ADADC44EEB28647C537C2BEB3

#include "ui_synchronisationsettings.h"
#include <KCModule>
#include <QVariantList>
#include <QList>
#include <QDateTime>

class KProgressDialog;
class QShowEvent;

class SynchronisationSettings : public KCModule
{
  Q_OBJECT
    private:
    Ui::SynchronisationSettingsDlg ui;
    KProgressDialog *dlg;

  private slots:
    void loadList();
    void displayList(const QList<QDateTime>& models);
    void modelSelectionChanged();
    void selectModel();
    void connected();
    void disconnected();

  protected:
    void showEvent(QShowEvent*);

  public:
    SynchronisationSettings(QWidget* parent, const QVariantList& args=QVariantList());
    ~SynchronisationSettings();
};
#endif
