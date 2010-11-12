/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#ifndef SIMON_COREEVENTS_H_5AB1C794A9624F49802445C0C89AB826
#define SIMON_COREEVENTS_H_5AB1C794A9624F49802445C0C89AB826

#include <QHash>
#include "clickmode.h"
#include "pressmode.h"

class QKeySequence;

class DeadKey
{
  private:
    unsigned int m_deadKey;
    unsigned int m_baseKey;
  public:
    DeadKey(unsigned int deadKey, unsigned int baseKey) {
      m_deadKey = deadKey;
      m_baseKey = baseKey;
    }

    unsigned int deadKey() const { return m_deadKey; }
    unsigned int baseKey() const { return m_baseKey; }
};

/**
 *	@class CoreEvents
 *	@brief Abstract class defining the methods that a EventBackend has to support
 *
 *	@version 0.1
 *	@date 4.03.2007
 *	@author Peter Grasch
 */
class CoreEvents
{

  enum KeyEvent
  {
    KeyDown=1,
    KeyUp=2
  };

  protected:
    QHash<unsigned int /*unicode char*/, DeadKey*> deadKeys;

    bool shiftSet, altgrSet, altSet, superSet, strgSet;
    bool shiftOnce, altgrOnce, altOnce, superOnce, strgOnce;
  public:
    CoreEvents();

    virtual void click(int x, int y, EventSimulation::ClickMode clickMode)=0;
    virtual void dragAndDrop(int xStart, int yStart, int x, int y)=0;

    void sendKey(unsigned int key /*unicode representation*/, EventSimulation::PressMode mode);
    virtual void sendKeyPrivate(unsigned int key /*unicode representation*/, EventSimulation::PressMode mode)=0;

    void unsetUnneededModifiers();
    void sendShortcut(const QKeySequence& shortcut, EventSimulation::PressMode mode);

    virtual void setModifierKey(int virtualKey, bool once=false)=0;
    virtual void unsetModifier(int virtualKey)=0;
    virtual ~CoreEvents() {}
};
#endif
