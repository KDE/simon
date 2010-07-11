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

class MODELMANAGEMENT_EXPORT Model
{
  private:
    qint32 m_data;
    QByteArray m_hmmDefs;
    QByteArray m_tiedList;
    QByteArray m_data1;
    QByteArray m_data2;

  public:
    Model(qint32 data, const QByteArray& hmmDefs,
      const QByteArray& tiedList, const QByteArray& data1, const QByteArray& data2);

    ~Model();

    qint32 sampleRate() { return m_data; }
    qint32 baseModelType() { return m_data; }
    QByteArray hmmDefs() { return m_hmmDefs; }
    QByteArray tiedList() { return m_tiedList; }
    QByteArray data1() { return m_data1; }
    QByteArray data2() { return m_data2; }

};
#endif
