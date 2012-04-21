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

#ifndef SIMON_MODEL_H_F94CE72E01134943B3FEA3B1D304768A
#define SIMON_MODEL_H_F94CE72E01134943B3FEA3B1D304768A

#include "simonmodelmanagement_export.h"
#include <QString>
#include <QHash>
#include <QStringList>
#include <QDateTime>

class KTar;

class MODELMANAGEMENT_EXPORT Model
{
  private:
    qint32 m_data;
    QByteArray m_container;
    
    //parsed form container
    bool m_containerParsed;
    QDateTime m_modelCreationDate;
    QString m_modelName;
    
    void parseContainer();

  public:
    Model(qint32 data, const QByteArray& container);

    qint32 sampleRate() { return m_data; }
    qint32 baseModelType() { return m_data; }
    QByteArray container() { return m_container; }
    
    static bool parseContainer(KTar& archive, QDateTime& creationDate, QString& name);
    
    QDateTime modelCreationDate();
    QString modelName();

};
#endif
