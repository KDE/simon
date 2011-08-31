/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#ifndef ATSPIACTION_H
#define ATSPIACTION_H
#include <QString>

class ATSPIAction
{
private:
  QString m_name;
  QString m_description;
  QString m_comment;
public:
  ATSPIAction (const QString& name, const QString& description, const QString& comment) :
    m_name(name), m_description(description), m_comment(comment)
  {}
  
  QString name() { return m_name; }
  QString description() { return m_description; }
  QString comment() { return m_comment; }
};

#endif // ATSPIACTION_H
