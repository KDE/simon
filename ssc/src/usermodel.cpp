/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#include "usermodel.h"
#include <sscobjects/user.h>
#include <QtGlobal>
#include <KLocalizedString>

UserModel::UserModel(QList<User*> users, QObject *parent) :
QAbstractItemModel(parent),
m_users(users)
{
}


/**
 * Replaces the data with the given one; The old list will be deleted;
 * This will reset the model to update all views accordingly
 */
void UserModel::replaceData(QList<User*> newUsers)
{
  qDeleteAll(m_users);

  this->m_users = newUsers;
  reset();
}


QString UserModel::displayStringForGrade(const int grade) const
{
  switch (grade) {
    case 1:
      return i18nc("Strings denoting a range from 1-5 (one being very good, the last one very bad; This is option 1", "Very good");
    case 2:
      return i18nc("Strings denoting a range from 1-5 (one being very good, the last one very bad; This is option 2", "Good");
    case 3:
      return i18nc("Strings denoting a range from 1-5 (one being very good, the last one very bad; This is option 3", "Satisfactory");
    case 4:
      return i18nc("Strings denoting a range from 1-5 (one being very good, the last one very bad; This is option 4", "Acceptable");
    case 5:
      return i18nc("Strings denoting a range from 1-5 (one being very good, the last one very bad; This is option 5", "Not acceptable");
  }

  return QString();
}


QString UserModel::displayStringBool(const bool b) const
{
  if (b) return i18nc("Possible answer to: Is x available?", "Yes");
  else return i18nc("Possible answer to: Is x available?", "No");
}


/*
 * Display: Id, Surname, Forename, Sex, Birthyear, ZIP Code, Orientation, Communication, MotorFucntion, InterviewPossible, RepeatPossible
 */
QVariant UserModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole) {
    User *u = m_users.at(index.row());
    switch (index.column()) {
      case 0:
        return u->userId();
      case 1:
        return u->surname();
      case 2:
        return u->givenName();
      case 3:
        return QString(QChar(u->sex()));
      case 4:
        return u->birthYear();
      case 5:
        return u->zipCode();
      case 6:
        return u->currentOccupation();
      case 7:
        return u->motherTongue();
      case 8:
        return displayStringForGrade(u->orientation());
      case 9:
        return displayStringForGrade(u->communication());
      case 10:
        return displayStringForGrade(u->motorFunction());
      case 11:
        return displayStringBool(u->interviewPossible());
      case 12:
        return displayStringBool(u->repeatingPossible());
    }
  }

  return QVariant();
}


Qt::ItemFlags UserModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


/*
 * Display: Id, Surname, Forename, Sex, Birthyear, ZIP Code, Orientation, Communication, MotorFucntion, InterviewPossible, RepeatPossible
 */
QVariant UserModel::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Id");
      case 1:
        return i18n("Surname");
      case 2:
        return i18n("Forename");
      case 3:
        return i18n("Sex");
      case 4:
        return i18n("Birthyear");
      case 5:
        return i18n("ZIP Code");
      case 6:
        return i18n("Occupation");
      case 7:
        return i18n("Mothertongue");
      case 8:
        return i18n("Orientation");
      case 9:
        return i18n("Communication");
      case 10:
        return i18n("Motor function");
      case 11:
        return i18n("Interview Possible");
      case 12:
        return i18n("Repeat Possible");
    }
  }

  //default
  return QVariant();
}


QModelIndex UserModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int UserModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return m_users.count();
  else return 0;
}


int UserModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 13;
}


QModelIndex UserModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, m_users.at(row));
}
