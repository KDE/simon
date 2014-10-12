/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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


#include "recognitioncontrolfactory.h"
#include "juliuscontrol.h"
#ifdef BACKEND_TYPE_BOTH
 #include "sphinxcontrol.h"
#endif

#include <KConfig>
#include <KStandardDirs>
#include <KConfigGroup>

RecognitionControlFactory::RecognitionControlFactory()
  : m_isolatedMode(false)
{
}

uint qHash(const ModelIdentifier& identifier)
{
  return qHash(identifier.userName) ^ (int) identifier.backend;
}

void RecognitionControlFactory::setIsolatedMode(bool isolatedMode)
{
    kDebug() << "Isolated Mode " << (isolatedMode?"ON":"OFF");
    m_isolatedMode = isolatedMode;
}

//TODO: account for getCapabilities() & ConcurrentSamples
RecognitionControl* RecognitionControlFactory::recognitionControl(const QString& user, RecognitionControl::BackendType type)
{
  RecognitionControl *r = NULL;
  ModelIdentifier id(user, type);
  if (m_isolatedMode || m_recognitionControls.count(id) == 0)
  {
    kDebug() << "RecognitionControls: generate new RC...";

    if(type == RecognitionControl::SPHINX)
    {
      #ifdef BACKEND_TYPE_BOTH
        r = new SphinxControl(user);
      #else
        return 0;
      #endif
    }
    else if(type == RecognitionControl::HTK)
      r = new JuliusControl(user);
    else
      return 0;

    m_recognitionControls.insert(id, r);
    kDebug() << "RecognitionControls: Inserted for User \"" << user << "\" [" << r << "] new RC... new user count: : " << QString::number(m_recognitionControls.count(id));
  } else /* isolatedMode = false and count > 0 (count = 1) */ {
    kDebug() << "RecognitionControls: use existing RC... count:" << m_recognitionControls.values(id).size();
    r = m_recognitionControls.values(id).first();
    kDebug() << "RecognitionControls: Using for User \"" << user << "\" [" << r << "] existing RC";
  }
  r->push();
  return r;
}

void RecognitionControlFactory::closeRecognitionControl(RecognitionControl* r)
{
  r->pop();
  if (r->isEmpty()) {
    m_recognitionControls.remove(ModelIdentifier(r->user(), r->type()));
    kDebug() << "RecognitionControls: Removed";
    delete r;
  } else {
    kDebug() << "RecognitionControls: Kept";
  }
}
