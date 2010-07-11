/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SSC_USERMODEL_H_56C23B99EA634CDD9EAC3A8957657818
#define SIMON_SSC_USERMODEL_H_56C23B99EA634CDD9EAC3A8957657818

#include <QList>
#include <sscobjects/user.h>
#include <QAbstractItemModel>

class UserModel : public QAbstractItemModel
{

  private:
    QList<User*> m_users;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;

    QString displayStringForGrade(const int grade) const;
    QString displayStringBool(const bool b) const;

  protected:
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public:
    explicit UserModel(QList<User*> users, QObject *parent=0);
    ~UserModel() {qDeleteAll(m_users); }
    void replaceData(QList<User*> newUsers);

};
#endif
