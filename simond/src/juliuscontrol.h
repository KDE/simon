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
 *
 *   In addition, as a special exception, the copyright holders give
 *   permission to link this portion of this program with the
 *   Julius library and distribute linked combinations including the two.
 *
 *   You must obey the GNU General Public License in all respects
 *   for all of the code used other than Julius.  If you modify
 *   file(s) with this exception, you may extend this exception to your
 *   version of the file(s), but you are not obligated to do so.  If you
 *   do not wish to do so, delete this exception statement from your
 *   version. 
 *
 *
 *   Powered By:
 *
 *   Large Vocabulary Continuous Speech Recognition Engine Julius
 *   Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan
 *   Copyright (c) 1991-2010 Kawahara Lab., Kyoto University
 *   Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 *   Copyright (c) 2005-2010 Julius project team, Nagoya Institute of Technology
 *
 */

#ifndef SIMON_JULIUSCONTROL_H_1A3B1D09EFF241FDAFC53C895B0017DD
#define SIMON_JULIUSCONTROL_H_1A3B1D09EFF241FDAFC53C895B0017DD

#include <QList>
#include <QPointer>
#include <KDebug>
#include "recognitioncontrol.h"

#ifdef bzero
#undef bzero
#endif

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

extern "C"
{
  #include <julius/julius.h>
}


class JuliusControl : public RecognitionControl
{
  Q_OBJECT

  public:
    enum Request
    {
      None=0,
      Stop=2
    };

    explicit JuliusControl(const QString& username, QObject *parent=0);

    bool initializeRecognition();

    bool startRecognition();
    bool stop();
    bool isInitialized();

    bool isStopping() { return stopping; }

    void waitForResumed();
    void recognized(const QString& fileName, const QList<RecognitionResult>& recognitionResults);
    JuliusControl::Request popNextRequest();

    Jconf* setupJconf();
    void recognize(const QString& fileName);

    ~JuliusControl();

  protected:
    void run();
    void pushRequest(JuliusControl::Request);
    void uninitialize();

  private:
    Recog *recog;
    Jconf *jconf;
    bool stopping;
    bool m_initialized;
    bool shouldBeRunning;
    FILE *logFile;

    QString currentFileName;

    QList<JuliusControl::Request> nextRequests;
    QByteArray getBuildLog();
    void emitError(const QString& error);
    void closeLog();
    bool stopPrivate();
    bool startRecognitionPrivate();

};
#endif
