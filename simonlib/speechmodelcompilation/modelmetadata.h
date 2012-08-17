/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#ifndef MODELMETADATA_H
#define MODELMETADATA_H

#include "simonmodelcompilationmanagement_export.h"
#include <QString>
#include <QDateTime>
#include <QDomDocument>

/*!
 * \class ModelMetadata
 * \brief ModelMetadata store information about specific acoustic model.
 *
 *  Model name, compilation time and type are stored.
 */
class MODELCOMPILATIONMANAGEMENT_EXPORT ModelMetadata
{
public:

  ModelMetadata(const QString &name, const QDateTime &time, QString type):m_Name(name), m_Time(time), m_Type(type){}
  ModelMetadata(const QDomElement &DomElement);
  ModelMetadata(){}

  QString name() const { return m_Name; }
  QDateTime dateTime() const { return m_Time; }
  QString type() const { return m_Type; }

  void setName(const QString &name) { m_Name = name; }
  void setTime(const QDateTime &time) {m_Time = time; }
  void setType(QString type) { m_Type = type; }

  QDomElement serializeXml(QDomDocument &DomDocument) const;
  bool deserializeXml(const QDomElement &DomElement);

protected:
  QString m_Name;
  QDateTime m_Time;
  QString m_Type;
};

#endif // MODELMETADATA_H
