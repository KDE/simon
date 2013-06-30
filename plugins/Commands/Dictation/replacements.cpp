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

#include "replacements.h"
#include "replacement.h"
#include <QRegExp>
#include <QDomElement>
#include <QDomDocument>
#include <KDebug>
#include <KLocalizedString>

Replacements::Replacements(QObject* parent) : QAbstractItemModel(parent)
{
}

bool Replacements::deSerialize(const QDomElement& elem)
{
  defaults();

  QDomElement replElem = elem.firstChildElement();

  while (!replElem.isNull()) {
    m_replacements << new Replacement(replElem.attribute("from"), replElem.attribute("to"));
    replElem = replElem.nextSiblingElement();
  }
  reset();
  return true;
}

QDomElement Replacements::serialize(QDomDocument* doc)
{
  QDomElement replacementsElem(doc->createElement("replacements"));
  foreach (const Replacement* r, m_replacements) {
    QDomElement rElem(doc->createElement("replacement"));
    rElem.setAttribute("from", r->from().pattern());
    rElem.setAttribute("to", r->to());
    replacementsElem.appendChild(rElem);
  }
  return replacementsElem;
}

int Replacements::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 2;
}

QVariant Replacements::headerData(int idx, Qt::Orientation orientation, int role) const
{
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (idx) {
    case 0:
      return i18nc("Original recognized text for the replacement / Source", "From");
    case 1:
      return i18nc("Replacement text / Goal", "To");
  }
  return QVariant();
}

QVariant Replacements::data(const QModelIndex& index, int role) const
{
  if (role != Qt::DisplayRole || index.row() >= m_replacements.count())
    return QVariant();

  const Replacement *r = m_replacements.at(index.row());
  switch (index.column()) {
    case 0:
      return r->from().pattern();
    case 1:
      return r->to();
  }
  return QVariant();
}

int Replacements::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return m_replacements.count();
}

QModelIndex Replacements::index(int row, int column, const QModelIndex& parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_replacements.at(row));
}

QModelIndex Replacements::parent(const QModelIndex& ) const
{
  return QModelIndex();
}

QString Replacements::replace(const QString& input) const
{
  QString out = input;
  foreach (const Replacement* r, m_replacements)
    out.replace(r->from(), r->to());
  return out;
}
void Replacements::add(Replacement* r)
{
  kDebug() << "From: " << r->from().pattern();
  kDebug() << "to: " << r->to();
  m_replacements.append(r);
  reset();
}

void Replacements::defaults()
{
  QList<Replacement*> old;
  old.append(m_replacements);
  m_replacements.clear();
  reset();
  qDeleteAll(old);
}

void Replacements::remove(Replacement* r)
{
  m_replacements.removeAll(r);
  delete r;
  reset();
}

Replacements::~Replacements()
{
  qDeleteAll(m_replacements);
}
