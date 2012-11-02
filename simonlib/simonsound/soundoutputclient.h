/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SOUNDOUTPUTCLIENT_H_4AC60251BE6A419BA1236280815A2AAD
#define SIMON_SOUNDOUTPUTCLIENT_H_4AC60251BE6A419BA1236280815A2AAD

#include <QIODevice>
#include <QBuffer>
#include "soundclient.h"
#include "simonsound_export.h"

class SIMONSOUND_EXPORT SoundOutputClient : public SoundClient
{
  private:
    QBuffer localBuffer;

  protected:
    virtual QIODevice* getDataProvider()=0;

  public:
    SoundOutputClient(const SimonSound::DeviceConfiguration& deviceConfiguration);
    virtual ~SoundOutputClient();

    void initToSampleRate(int contentSampleRate);

    qint64 read(char *data, qint64 maxlen);

    virtual void finish()=0;
};
#endif
