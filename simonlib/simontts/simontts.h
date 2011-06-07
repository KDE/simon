/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SIMONTTS_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_SIMONTTS_H_7A7B9100FF5245329569C1B540119C37

#include "simontts_export.h"
#include <QFlags>
#include <QStringList>

class QString;
class SimonTTSPrivate;

/**
 * \class SimonTTS
 * \author Peter Grasch
 * \since 0.4
 * \brief Provides access to text to speech functionality
 */
class SIMONTTS_EXPORT SimonTTS
{
  protected:
    static SimonTTSPrivate *d;

  public:
    static SimonTTSPrivate* getInstance();

    enum TTSFlag
    {
      None=0,
      StripHTML=1
    };
    Q_DECLARE_FLAGS(TTSFlags, TTSFlag);

    static bool initialize();
    static bool uninitialize();
    static bool say(const QString& text, SimonTTS::TTSFlags flags=SimonTTS::StripHTML);
    static bool interrupt();
    static QStringList recentlyUsed();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SimonTTS::TTSFlags);
#endif

