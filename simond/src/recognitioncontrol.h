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

#ifndef SIMON_RECOGNITIONCONTROL_H_F4CB407566D6459487054C2B03311E38
#define SIMON_RECOGNITIONCONTROL_H_F4CB407566D6459487054C2B03311E38

#include <QObject>
#include <QThread>
#include <QList>
#include <QDateTime>
#include <QMetaType>
#include <simonrecognitionresult/recognitionresult.h>

class RecognitionControl : public QThread
{
  Q_OBJECT

  private:
    QDateTime m_lastSuccessfulStart;
    QDateTime m_lastFailedStart;

  signals:
    void recognitionReady();
    void recognitionError(const QString& error, const QByteArray& buildLog);
    void recognitionWarning(const QString& warning);
    void recognitionStarted();
    void recognitionStopped();
    void recognitionPaused();
    void recognitionResumed();
    void recognitionResult(const QString& fileName, const RecognitionResultList& recognitionResults);

  private slots:
    void touchLastFailedStart();

  protected:
    void touchLastSuccessfulStart();
    QString username;

  public:
    //QDateTime lastSuccessfulStart() { return m_lastSuccessfulStart; }
    bool shouldTryToStart(const QDateTime& activeModelDate);
    explicit RecognitionControl(const QString& username, QObject *parent=0);

    virtual bool initializeRecognition()=0;
    virtual bool isInitialized()=0;
    virtual bool startRecognition()=0;
    virtual void stop()=0;

    virtual void recognize(const QString& fileName)=0;

    ~RecognitionControl();

};
#endif
