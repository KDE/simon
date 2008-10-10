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


#ifndef WAV_H
#define WAV_H

#include <QString>
#include <QBuffer>


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
class WAV{
private:
	QBuffer wavData;
	unsigned long length; //!< this is needed as there seems to be no way to determine the length of an array
	int samplerate; //!< the samplerate of the file
	int channels;
	QString filename; //!< Filename
	
	void writeHeader(QDataStream *dstream);
	void writeFormat(QDataStream *dstream);
	void writeDataChunk(QDataStream *dstream);
	void importDataFromFile(QString filename);
	int retrieveSampleRate();
	int retrieveChannels();
	
public:
    explicit WAV(QString filename,int channels=0, int samplerate=0);

	bool beginAddSequence() {
		return wavData.open(QIODevice::WriteOnly|QIODevice::Append);
	}

	bool endAddSequence() {
		if (wavData.isOpen()) wavData.close();
		return true;
	}

	short* getRawData(unsigned long& length);
    void addData(short* data, int length);
	int getLength() { return length; }
    bool writeFile(QString filename="");
    int getSampleRate() { return samplerate; }
    int getChannels() { return channels; }
    ~WAV();

};

#endif
