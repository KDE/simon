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

RecognitionControl* RecognitionControlFactory::recognitionControl(const QString& user)
{
  if (!m_recognitionControls.contains(user)) {
    m_recognitionControls.insert(user, new JuliusControl(user));
  }
  RecognitionControl *r = m_recognitionControls.value(user);
  r->push();
  return r;
}

void RecognitionControlFactory::closeRecognitionControl(const QString& user)
{
  RecognitionControl *r = m_recognitionControls.value(user);
  r->pop();
  if (r->isEmpty()) {
    m_recognitionControls.remove(user);
    delete r;
  }
}
