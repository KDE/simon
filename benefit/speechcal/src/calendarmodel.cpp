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

#include "calendarmodel.h"

CalendarModel::CalendarModel(QObject* parent): QAbstractListModel(parent)
{
}

void CalendarModel::initialize(const QList<QSharedPointer<KCalCore::Event> > items)
{
  relevantItems = items;
  reset();
}

QVariant CalendarModel::data(const QModelIndex& index, int role) const
{
  if (role != Qt::DisplayRole) return QVariant();
  QSharedPointer< KCalCore::Event > event = relevantItems[index.row()];
  QString text = QString("%1 - %2: %3").arg(event->dtStart().toString("%H:%M")).arg(
						event->dtEnd().toString("%H:%M")).arg(event->summary());
  return text;
}

int CalendarModel::rowCount(const QModelIndex& /*parent*/) const
{
  return relevantItems.count();
}

void CalendarModel::clear()
{
  relevantItems.clear();
  reset();
}


