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

#ifndef SIMON_WebserviceTTSProvider_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_WebserviceTTSProvider_H_7A7B9100FF5245329569C1B540119C37

#include "simonttsprovider.h"
#include <QStringList>
#include <QObject>
#include <QBuffer>
#include <QSharedPointer>
#include <QQueue>
#include <QFile>
#include <QMutex>

class QString;
class WavPlayerClient;
class QBuffer;
class QNetworkReply;
class QNetworkAccessManager;

/**
 * \class WebserviceTTSProvider
 * \author Peter Grasch
 * \since 0.4
 * \brief Interface to pre recorded sound snippets
 */
class WebserviceTTSProvider : public QObject, public SimonTTSProvider
{
  Q_OBJECT
  private:
    QMutex downloadMutex;
    QQueue<QString> filesToDownload;
    QQueue< QSharedPointer<QBuffer> > filesToPlay;
    int downloadOffset; /// we are always playing the file at position 0 of filesToPlay; we are downloading file [0+downloadOffset]
    QNetworkReply *currentConnection;
    
    QNetworkAccessManager *net;
    WavPlayerClient *player;
    void enquePlayback();
    void fetch(const QString& url);

  private slots:
    void downloadProgress(qint64,qint64);
    void replyReceived();
    void playNext();
    void initializeOutput();

  public:
    WebserviceTTSProvider();
    bool initialize();
    bool uninitialize();
    bool canSay(const QString& text);
    bool say(const QString& text);
    bool interrupt();
    ~WebserviceTTSProvider();
};

#endif

