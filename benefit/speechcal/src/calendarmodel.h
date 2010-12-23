/*   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#ifndef SIMON_CALENDERMODEL_H_4002119636CC42C68FE07273F9000A73
#define SIMON_CALENDERMODEL_H_4002119636CC42C68FE07273F9000A73

#include <QModelIndex>
#include <QList>
#include <QSharedPointer>
#include <kcalcore/event.h>

class CalendarModel : public QAbstractListModel
{
private:
    QList< QSharedPointer<KCalCore::Event> > relevantItems;
  
public:
    CalendarModel(QObject* parent = 0);
    void initialize(const QList< QSharedPointer<KCalCore::Event> > items);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    void clear();
};

#endif // SIMON_CALENDERMODEL_H_4002119636CC42C68FE07273F9000A73
