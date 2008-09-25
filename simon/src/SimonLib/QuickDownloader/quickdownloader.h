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


#ifndef QUICKDOWNLOADER_H
#define QUICKDOWNLOADER_H

#include <QWidget>
#include <QHttpResponseHeader>


class QFile;
class QUrl;
class QHttp;
class KProgressDialog;

/**
 * \class QuickDownloader
 * \brief Downloadwidget that pops up, displays a progressdlg. and downloads the given url
 * \author Peter Grasch
 * \version 0.1
 * \date 02.06.2007
*/
class QuickDownloader : public QWidget {
	Q_OBJECT
signals:
	void startDownload(QString url);
	void progress(int now, int max);
	void downloadFinished(QString filename);
	void errorOccured(QString);
	void aborted();

private:
	KProgressDialog *progressDlg;
	QHttp *loader;
	int request;
	bool aborting;
	QFile *file;
	
private slots:
	void requestFinished(int id, bool error);
	void dataReceived(int now, int max);
	void readResponse(const QHttpResponseHeader header);
	
public slots:
	bool download(QString url, QString filename="");
	void cancelDownload();

public:
    QuickDownloader(QWidget *parent=0);
	void destroyFile();

    ~QuickDownloader();

};

#endif
