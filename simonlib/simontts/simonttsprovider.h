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

#ifndef SIMON_SimonTTSProvider_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_SimonTTSProvider_H_7A7B9100FF5245329569C1B540119C37

class QString;

/**
 * \class SimonTTSProvider
 * \author Peter Grasch
 * \since 0.4
 * \brief Abstract base class for every TTSProvider
 */
class SimonTTSProvider
{
  public:
    virtual bool initialize()=0;
    virtual bool uninitialize()=0;
    virtual bool canSay(const QString& text)=0;
    virtual bool say(const QString& text)=0;
    virtual bool interrupt()=0;
    virtual ~SimonTTSProvider() {}

};

#endif


