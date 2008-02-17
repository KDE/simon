//
// C++ Interface: quickdownloader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QUICKDOWNLOADER_H
#define QUICKDOWNLOADER_H

#include <QWidget>
#include <QProgressDialog>
#include <QHttp>
#include <QUrl>
#include <QHttpResponseHeader>
#include <QMessageBox>
#include <QFileInfo>


class QFile;

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
	QProgressDialog *progressDlg;
	QHttp *loader;
	int request;
	bool aborting;
	QFile *file;
	
private slots:
	void requestFinished(int id, bool error);
	void dataRecieved(int now, int max);
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
