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


#ifndef SIMON_DIALOGTEMPLATEOPTIONS_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_DIALOGTEMPLATEOPTIONS_H_4B4956DCAE204C49977297D20CB81F09

#include <QMap>
#include <QString>
#include <QDomElement>
#include <QAbstractItemModel>

class DialogTemplateOptions : public QAbstractItemModel
{
  private:
    QMap<QString, bool> options;
    DialogTemplateOptions();

  protected:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
    QObject* parent() { return QObject::parent(); }
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public:
    static DialogTemplateOptions* createInstance(const QDomElement& elem);
    QDomElement serialize(QDomDocument *doc);
    bool deSerialize(const QDomElement& elem);

    void addOption(const QString& name, bool enabled);
    void removeOption(const QString& name);
    bool isEnabled(const QString& name) { return options.value(name); }
};

#endif

