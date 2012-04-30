/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "model.h"
#include <QBuffer>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <KFilterDev>
#include <KTar>

Model::Model(qint32 data, const QByteArray& container) :
m_data(data), m_container(container), m_containerParsed(false)
{
}

QDateTime Model::modelCreationDate()
{
  if (!m_containerParsed) parseContainer();
  return m_modelCreationDate;
}

QString Model::modelName()
{
  if (!m_containerParsed) parseContainer();
  return m_modelName;
}

bool Model::parseContainer ( KTar& archive, QDateTime& creationDate, QString& name )
{
  if (archive.open(QIODevice::ReadOnly)) {
    const KArchiveDirectory *d = archive.directory();
    if (d) {
      const KArchiveFile *entry = dynamic_cast<const KArchiveFile*>(d->entry("metadata.xml"));
      if (entry) {
        QDomDocument doc;
        doc.setContent(entry->data());
        QDomElement rootElem = doc.documentElement();
        QDomElement nameElem = rootElem.firstChildElement("name");
        QDomElement dateElem = rootElem.firstChildElement("creationDate");
        if (!nameElem.isNull() && !dateElem.isNull()) {
          creationDate = QDateTime::fromString(dateElem.text(), Qt::ISODate);
          name = nameElem.text();
          return true;
        }
        else kDebug() << "Elements 0";
      }
        else kDebug() << "Entry invalid";
    }
        else kDebug() << "Directory invalid";
  }
        else kDebug() << "Couldn't open tar";
        
  return false;
}

void Model::parseContainer()
{
  if (!m_container.isNull()) {
    QBuffer model(&m_container);
    model.open(QIODevice::ReadOnly);
    QIODevice *filter = KFilterDev::device(&model, "application/x-gzip", false /* will be deleted when unwinding stack */);
    filter->open(QIODevice::ReadOnly);
    QByteArray uncompressed = filter->readAll();
    delete filter; //KTar doesn't work with KFilterDev for some reason...
    
    QBuffer uncompressedModel(&uncompressed);
    uncompressedModel.open(QIODevice::ReadOnly);
    KTar archive(&uncompressedModel);
    parseContainer(archive, m_modelCreationDate, m_modelName);
  }
  m_containerParsed = true;
}
