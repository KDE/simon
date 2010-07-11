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

#ifndef SIMON_SIMONDUSERCONFIGURATION_H_5B17417E5890427EA6664411B05B2308
#define SIMON_SIMONDUSERCONFIGURATION_H_5B17417E5890427EA6664411B05B2308

#include <KCModule>
#include <QVariantList>

#include "ui_simonduserconfiguration.h"

class DatabaseAccess;

class SimondUserConfiguration : public KCModule
{
  Q_OBJECT

    private:
    bool alreadyLoaded;
    Ui::UserConfiguration ui;
    DatabaseAccess *db;
    QString storedDb;

    void activateUserConfiguration(bool active);
    void initDb();
    QByteArray encryptPassword(const QString& pass);

    bool recursiveDelete(const QString& dirPath);

  private slots:
    void addUser();
    void deleteUser();
    void changePassword();
    void slotChanged();

  public:
    SimondUserConfiguration(QWidget* parent, const QVariantList& args=QVariantList());
    ~SimondUserConfiguration();

    void load();
    void save();
};
#endif
