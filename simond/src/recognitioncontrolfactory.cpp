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


#include "recognitioncontrolfactory.h"
#include "juliuscontrol.h"
#include "sphinxcontrol.h"
#include <KConfig>
#include <KStandardDirs>
#include <KConfigGroup>

RecognitionControlFactory::RecognitionControlFactory()
  : m_isolatedMode(false)
{
}

void RecognitionControlFactory::setIsolatedMode(bool isolatedMode)
{
    kDebug() << "Isolated Mode " << (isolatedMode?"ON":"OFF");
    m_isolatedMode = isolatedMode;
}

RecognitionControl* RecognitionControlFactory::recognitionControl(const QString& user)
{
  RecognitionControl *r = NULL;
  if (m_isolatedMode || m_recognitionControls.count(user) == 0) {
    kDebug() << "RecognitionControls: generate new RC...";

    KConfig config( KStandardDirs::locateLocal("config", "simonmodelcompilationrc"), KConfig::FullConfig );
    KConfigGroup programGroup(&config, "Backend");
    bool sphinx,jhtk;
    sphinx = programGroup.readEntry("sphinx", true);
    jhtk = programGroup.readEntry("jhtk", false);

    if(sphinx)
      r = new SphinxControl(user);
    else if(jhtk)
      r = new JuliusControl(user);
    else
    {};//;(

    m_recognitionControls.insert(user, r);
    kDebug() << "RecognitionControls: Inserted for User \"" << user << "\" [" << r << "] new RC... new user count: : " << QString::number(m_recognitionControls.count(user));
  } else /* isolatedMode = false and count > 0 (count = 1) */ {
    kDebug() << "RecognitionControls: use existing RC... count:" << m_recognitionControls.values(user).size();
    r = m_recognitionControls.values(user).first();
    kDebug() << "RecognitionControls: Using for User \"" << user << "\" [" << r << "] existing RC";
  }
  r->push();
  return r;
}

void RecognitionControlFactory::closeRecognitionControl(const QString& user, RecognitionControl* r)
{
  r->pop();
  if (r->isEmpty()) {
    m_recognitionControls.remove(user, r);
    kDebug() << "RecognitionControls: Removed for User \"" << user << "\" [" << r << "] existing RC... new user count: " << QString::number(m_recognitionControls.count(user));
    delete r;
  } else {
    kDebug() << "RecognitionControls: Keep for User \"" << user << "\" [" << r << "] existing RC ... still in use";
  }
}
