/*  
    Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
    Copyright (C) 2008-2009 Pali Rohár <pali.rohar@gmail.com>

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License version 2 as published by the Free Software Foundation.

    This application is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/
#ifndef SIMONSKYPE_H
#define SIMONSKYPE_H

#include <QObject>
#include "skype.h"

class SimonSkype : public QObject
{
  Q_OBJECT
  private:
    bool dropVoiceMail;
    Skype s;

  private slots:
    void newCall(const QString& callId, const QString& userId);
    void callStatus(const QString& callId, const QString& status);
    void voiceMailActive(int id);
    void voiceMessageSent();

  public:
    SimonSkype();

};

#endif

