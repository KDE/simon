/*
 *   Copyright (C) 2013 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_REPLACEMENTS_H_DF2B936F15A84FD8962EF68E1DAFE24F
#define SIMON_REPLACEMENTS_H_DF2B936F15A84FD8962EF68E1DAFE24F

#include <QAbstractItemModel>
#include <QList>
#include <QDomElement>
class Replacement;
class QDomDocument;

class Replacements: public QAbstractItemModel
{
  Q_OBJECT

public slots:
  virtual bool deSerialize(const QDomElement&);
  virtual QDomElement serialize(QDomDocument *doc);
  void defaults();

public:
  explicit Replacements(QObject *parent=0);
  ~Replacements();

  QString replace(const QString& input) const;
  void add(Replacement *r);
  void remove(Replacement *r);

protected:
  virtual QVariant data(const QModelIndex &index, int role) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex&) const;
private:
  QList<Replacement*> m_replacements;

  QVariant headerData(int, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
};
#endif
