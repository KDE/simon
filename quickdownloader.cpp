//
// C++ Implementation: quickdownloader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "quickdownloader.h"

QuickDownloader::QuickDownloader(QWidget *parent) : QWidget(parent)
{
	this->loader = new QHttp();
	this->progressDlg = new QProgressDialog(this);
	file = new QTemporaryFile(this);
}

bool QuickDownloader::download(QString url)
{
	if (!loader || !progressDlg) return false;
	
	QUrl urlD = QUrl(url);
	
	QHttp::ConnectionMode mode = urlD.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	
	if (!urlD.userName().isEmpty())
		loader->setUser(urlD.userName(), urlD.password());
	
	
	connect (loader, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
	connect(loader, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataRecieved(int, int)));
	connect(progressDlg, SIGNAL(canceled()), this, SLOT(cancelDownload()));
	connect(loader, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
		this, SLOT(readResponse(const QHttpResponseHeader &)));
	
	
	loader->setHost(urlD.host(), mode, urlD.port() == -1 ? 0 : urlD.port());;
	QFileInfo info = QFileInfo(urlD.path());
	
	progressDlg->setWindowTitle("Lade "+info.fileName());
	progressDlg->setLabelText("Lade "+url);
	progressDlg->setMaximum(-1);
	progressDlg->setValue(-1);
	progressDlg->show();
	
	aborting = false;
	if (!file) return false;
	if (!file->open())
	{
		
		QMessageBox::critical(this, tr("Fehler beim Öffnen"),
				QString("Konnte die temporäre Datei (%1) nicht öffnen:\n%2")
				.arg(file->fileName()).arg(file->errorString()));
		return false;
	}
	this->filename = file->fileName();
	
	this->request = loader->get(urlD.path(), file);
	
	return true;
}

void QuickDownloader::readResponse(const QHttpResponseHeader header)
{
	if (header.statusCode() != 200) {
		QMessageBox::information(this, tr("HTTP"),
					 tr("Download failed: %1.")
							 .arg(header.reasonPhrase()));
		aborting = true;
		progressDlg->hide();
		loader->abort();
		return;
	}
}

void QuickDownloader::cancelDownload()
{
	aborting = true;
	loader->abort();
}

void QuickDownloader::dataRecieved(int now, int max)
{
	if (aborting) return;
	
	progressDlg->setMaximum(max);
	progressDlg->setValue(now);
	emit progress(now, max);
}

void QuickDownloader::requestFinished(int id, bool error)
{
	if (error) emit errorOccured(loader->errorString());
	
	if ((id == this->request) && (aborting == false)) {
		if (file)
			file->close();
		progressDlg->hide();
		emit downloadFinished(filename);
	}
	if (aborting)
	{
		if (file) {
			file->close();
			file->remove();
		}
		progressDlg->hide();
		emit aborted();
	}
}


QuickDownloader::~QuickDownloader()
{
}


