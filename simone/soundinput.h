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

#ifndef SOUNDINPUT_H
#define SOUNDINPUT_H

#include <QIODevice>
#include <QMutex>

class QAudioInput;

class SoundInput : public QIODevice
{
    Q_OBJECT
signals:
    void microphoneLevel(int level, int min=-1, int max=-1);

    void error(const QString& error);

    void listening();
    void complete();

private:
    QAudioInput *input;
    qint8 m_channels;
    qint32 m_sampleRate;

    QMutex bufferLock;
    QByteArray localBuffer;


    //VAD
    int m_currentTime;

    int m_peak;
    int m_average;
    int m_absolutePeak;
    int m_absoluteMinAverage;
    bool m_clipping;

    qint64 lastLevel;
    qint64 lastTimeUnderLevel;
    qint64 lastTimeOverLevel;

    bool waitingForSampleToStart;
    bool waitingForSampleToFinish;
    bool currentlyRecordingSample;

    QByteArray currentSample; //private buffer for head

    inline int bytesToLength(int bytes);
    inline int lengthToBytes(int length);
    inline int getLengthFactor();
    void analyzeSoundProperties(const QByteArray& data);
    void process(QByteArray& data);
    //END VAD

protected:
    qint64 readData(char* data, qint64 maxSize);
    qint64 writeData(const char* data, qint64 maxSize);

private slots:
    void stateChanged();

public:
    SoundInput(int channels, int sampleRate, QObject *parent);

    bool init();

    qint8 channels() { return m_channels; }
    qint32 sampleRate() { return m_sampleRate; }

    void dropCache();

    int maxAmp() { return 32768; }
    int average() { return m_average; }
    int peak() { return m_peak; }
    int absoluteMinAverage() { return m_absoluteMinAverage; }
    int absolutePeak() { return m_absolutePeak; }
    bool clipping() { return m_clipping; }
};

#endif // SOUNDINPUT_H
