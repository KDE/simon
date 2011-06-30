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

#ifndef SIMON_WAV_H_E9CC980F82B347F2B4677268B81B9D1F
#define SIMON_WAV_H_E9CC980F82B347F2B4677268B81B9D1F

#include <QString>
#include <QBuffer>

#include "simonwav_export.h"

/**
 *	@class WAV
 *	@brief The WAV Filewriter
 *
 *	This class provides all the needed utilities to write a standard compliant
 *	WAV File out of raw PCM data (such as the data provided by SoundControl,etc.).
 *
 *	@version 0.1
 *	@date 17.03.2007
 *	@author Peter Grasch
 *
 *	@todo We use a single buffer that we resize every time; If it gets too large to handle we get "skippy" recordings
 */
class SIMONWAV_EXPORT WAV : public QBuffer
{
  private:
    //	QBuffer wavData;
    unsigned long length;                         //!< this is needed as there seems to be no way to determine the length of an array
    qint32 samplerate;                               //!< the samplerate of the file
    qint16 channels;
    QString filename;                             //!< Filename

    void writeHeader(QDataStream *dstream);
    void writeFormat(QDataStream *dstream);
    void writeDataChunk(QDataStream *dstream);
    void importDataFromFile(QIODevice* device, bool skipHeader=false);
    int retrieveSampleRate();
    int retrieveChannels();

  protected:
    virtual qint64 writeData ( const char * data, qint64 maxSize );

  public:
    static bool parseHeader(QIODevice* device, qint16& channels, qint32& samplerate);
    explicit WAV(QString filename,int channels=0, int samplerate=0);

    bool beginAddSequence() {
      if (isOpen())
        close();
      return open(QIODevice::WriteOnly|QIODevice::Append);
    }

    bool endAddSequence() {
      if (isOpen())
        close();
      return true;
    }

    bool beginReadSequence() {
      if (isOpen())
        close();
      return open(QIODevice::ReadOnly);
    }
    bool endReadSequence() {
      if (isOpen())
        close();
      return true;
    }

    void addData(short* data, int length);

    QString getFilename() { return filename; }
    int getLength() { return length; }
    bool writeFile(QString filename="");
    int getSampleRate() { return samplerate; }
    int getChannels() { return channels; }
    ~WAV();

};
#endif
