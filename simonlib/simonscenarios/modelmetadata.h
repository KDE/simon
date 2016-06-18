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

#include "simonscenarios_export.h"
#include <QString>
#include <QDateTime>
#include <QDomDocument>
#include <QStringList>

/*!
 * \class ModelMetadata
 * \brief ModelMetadata store information about specific acoustic model.
 *
 *  Model name, compilation time and type are stored.
 */
class SIMONSCENARIOS_EXPORT ModelMetadata
{
public:

  ModelMetadata(const QString &name, const QDateTime &time, const QString& type, const QStringList& droppedPronunciations) :
              m_name(name), m_time(time), m_type(type), m_droppedPronunciations(droppedPronunciations) {}
  ModelMetadata(const QDomElement &domElement);
  ModelMetadata(){}

  QString name() const { return m_name; }
  QDateTime dateTime() const { return m_time; }
  QString type() const { return m_type; }
  QStringList droppedPronunciations() const { return m_droppedPronunciations; }

  void setName(const QString &name) { m_name = name; }
  void setTime(const QDateTime &time) {m_time = time; }
  void setType(QString type) { m_type = type; }

  QDomElement serializeXml(QDomDocument &domDocument) const;
  bool deserializeXml(const QDomElement &domElement);

protected:
  QString m_name;
  QDateTime m_time;
  QString m_type;
  QStringList m_droppedPronunciations;
};

#endif // MODELMETADATA_H
