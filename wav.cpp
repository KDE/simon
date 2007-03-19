//
// C++ Implementation: wav
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "wav.h"

/**
 *	@brief Constructor
 *	
 *	Initializes the member variables
 *
 *	@author Peter Grasch
 *	@param QString filename
 *	The filename of the .wav file
 *	@param int samplerate
 *	The samplerate of the file
 *	
 */
WAV::WAV(QString filename, int samplerate)
{
	this->filename = filename;
	this->samplerate = samplerate;
	waveData = new char();
	length = 0;
}

/**
 *	@brief Writes the file
 *	
 *	Calls all the submethods to write the header, writes the data stored in
 *	waveData (member) and closes the file.
 *
 *	@author Peter Grasch
 *	@param QString filename
 *	The filename - if omitted the filename (member) is used
 */
bool WAV::writeFile(QString filename)
{
	if (filename.isEmpty()) filename = this->filename;
	
	QFile wavFile(filename);
	wavFile.open(QIODevice::WriteOnly);
	QDataStream *dstream = new QDataStream(&wavFile);
	
	dstream->setByteOrder( QDataStream::LittleEndian );
	
	writeHeader(dstream);
	writeFormat(dstream);
	writeDataChunk(dstream);
	
	dstream->writeRawData(waveData, this->length);
	
	wavFile.close();
	return true;
}


/**
 *	@brief Writes the basic WAV-header
 *	
 *	This identifies the WAV file as WAV file.
 *	
 *	+------------------------------------------------------+
 *	| CHAR|Length of Byte to follow (16bit)|CHAR           |
 *	|------------------------------------------------------|
 *	| RIFF|         18276                  |WAVE           |
 *	+------------------------------------------------------+
 *
 *	@author Peter Grasch
 *	@param QDataStream *dstream
 *	All data is written to that stream
 */
void WAV::writeHeader(QDataStream *dstream)
{
	dstream->writeRawData("RIFF",4);
	*dstream << (quint16) ((this->length)*2)+36;
	dstream->writeRawData("WAVE",4);
}

/**
 *	@brief Writes the header of one data chunk to the provided QDataStream
 *	
 *	This identifies the WAV file as WAV file.
 *	
 *	+------------------------------------------------------+
 *	| CHAR|Length of Byte to follow (16bit)                |
 *	|------------------------------------------------------|
 *	| data|         18276                                  |
 *	+------------------------------------------------------+
 *
 *	@author Peter Grasch
 *	@param QDataStream *dstream
 *	All data is written to that stream
 */
void WAV::writeDataChunk(QDataStream *dstream)
{
	dstream->writeRawData("data",4);
	*dstream << (quint32) (this->length)*2;
}

/**
 *	@brief Writes the format chunk to the QDataStream
 *	
 *	The format chunk defines how to interpret the data chunk.
 *	
 *	+-------------------------------------------------------+
 *	| CHAR|Chunk Size|Compression Code*|#channels|samplerate|
 *	|-------------------------------------------------------|
 *	| fmt |0x10      |0x01             |0x02     |44100     |
 *	+-------------------------------------------------------+
 *
 *	+-------------------------------------------------------+
 *	| Bytes/sec|Block align|Bits/Sample                     |
 *	|-------------------------------------------------------|
 *	| 88200    |4          |16                              |
 *	+-------------------------------------------------------+
 *	
 *	* 0x01 means uncompressed PCM Data
 *
 *	@author Peter Grasch
 *	@param QDataStream *dstream
 *	All data is written to that stream
 */
void WAV::writeFormat(QDataStream *dstream)
{
	dstream->writeRawData("fmt ",4);
	*dstream << (quint32) 0x10 << (quint16) 0x01 << (quint16) 0x02 <<
			(quint32) 44100 << (quint32) 88200 << (quint16) 4 << (quint16) 16;
}

/**
 *	@brief Adds the array of data to the main waveData (member)
 *
 *	@author Peter Grasch
 *	@param char* data
 *	the data to add
 *	@param int length
 *	the length of the provided array
 */
void WAV::addData(char* data, int length)
{
	
	//saving the wavedata temp.
	char* tmp = (char*) malloc (this->length);
	for (int i = 0; i < this->length; i++)
		tmp[i] = waveData[i];
	
	//reallocate the member and extends its size of the size of the data to add
	waveData = (char*) malloc(this->length + length);
	
	//copy back the original data
	for (int i = 0; i < this->length; i++)
		waveData[i] = tmp[i];
	
	//add the new data
	for (int i = 0; i < length; i++)
		waveData[i+this->length] = data[i];
	
	this->length += length;
}

