/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef CACHEDMODEL_H
#define CACHEDMODEL_H

#include "contextadapter.h"

#include <QDateTime>

class CachedModel
{
public:
  enum ModelState {
    Current=1,
    ToBeEvaluated=2,
    Building=4,
    Null=9
  };

  CachedModel(const QDateTime& compiledDate, ModelState state, uint fingerPrint, ContextAdapter::BackendType type);

  QDateTime compiledDate() const { return m_compiledDate; }
  ModelState state() const { return m_state; }
  uint srcFingerPrint() const { return m_srcFingerPrint; }
  ContextAdapter::BackendType type() const { return m_type; }

  void setState(ModelState state) { m_state = state; }
  void setSrcFingerPrint ( uint fingerprint );
  void setCompiledDate(const QDateTime& compiled);
  void setType(ContextAdapter::BackendType type);

private:
  QDateTime m_compiledDate;
  ModelState m_state;
  uint m_srcFingerPrint;
  ContextAdapter::BackendType m_type;
};

#endif // CACHEDMODEL_H
