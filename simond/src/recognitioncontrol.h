/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include <simonrecognitionresult/recognitionresult.h>
#include "simonrecognizer/recognizer.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QQueue>

/*!
 * \class RecognitionControl
 * \brief The RecognitionControl class - entity which works in separate thread. Main case - managing recognition.
 *  Initialize recognition. Accepts request for recognize some files. Indicates on the recognition results.\
 *
 *  \version 0.1
 *  \date 15.08.2012
 *  \author Vladislav Sitalo
 */
class RecognitionControl : public QThread
{
  Q_OBJECT

  signals:
    void recognitionReady();
    void recognitionError(const QString& error, const QByteArray& buildLog);
    void recognitionWarning(const QString& warning);
    void recognitionStarted();
    void recognitionStopped();
    void recognitionPaused();
    void recognitionResumed();
    void recognitionResult(const QString& fileName, const RecognitionResultList& recognitionResults);
    void recognitionDone(const QString& fileName);

  public:
    enum BackendType
    {
      HTK = 1,
      SPHINX = 2
    };
    explicit RecognitionControl(const QString& username, BackendType type, QObject *parent=0);

    virtual bool initializeRecognition(const QString& modelPath)=0;
    virtual bool isInitialized() { return m_initialized; }

    virtual bool startRecognition();

    virtual bool stop();
    virtual bool suspend(); //stop temporarily (still reflect the intention of being active, but don't do any recognition)

    /*!
     * \brief Add file name to recognition queue.
     * \param fileName File name.
     */
    virtual void recognize(const QString& fileName);

    bool recognitionRunning();

    void pop();
    void push();
    bool isEmpty() const;

    BackendType type() const { return m_type; }
    QString user() const { return username; }

    ~RecognitionControl();

  private:
    int m_refCounter;
    BackendType m_type;

  protected:
    QString m_lastModel;

  protected:
    QString username;
    int m_startRequests;
    bool m_initialized;

    QMutex queueLock;
    QQueue<QString> toRecognize;

    bool stopping;

    bool shouldBeRunning;

    QString currentFileName;

    virtual QByteArray getBuildLog();
    virtual bool stopInternal();
    virtual void uninitialize();
    virtual bool startRecognitionInternal();

    void run();

    virtual RecognitionConfiguration* setupConfig()=0;
    virtual void emitError(const QString& error)=0;

    Recognizer *recog;
};
#endif
