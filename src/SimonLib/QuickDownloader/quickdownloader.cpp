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
#include "logger.h"
#include <QTemporaryFile>
#include <QFile>
#include "settings.h"
#include <QVariant>
#include <QProgressDialog>
#include <QHttp>
#include <QUrl>
#include <QMessageBox>
#include <QFileInfo>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param QWidget *parent
 * The parent of the Widget - is passed on to the QWidget constructor
 */
QuickDownloader::QuickDownloader(QWidget *parent) : QWidget(parent)
{
	this->loader = new QHttp();
	this->progressDlg = new QProgressDialog(this);
	this->file = 0;
}

void QuickDownloader::destroyFile()
{
	delete file;
	file = 0;
}

/**
 * \brief Downloads the given URL
 * \author Peter Grasch
 * @param QString url
 * The url to the file to download
 * @return bool
 * if there is an error this is set to false
 */
bool QuickDownloader::download(QString url, QString filename)
{
	if (loader && (loader->state() != QHttp::Unconnected))
	{
		loader->abort();
		loader->close();
	}

	if (file)
		destroyFile();


	if (filename.isEmpty())
		file = new QTemporaryFile(Settings::getS("TempDir")+"/"+"simon_tmp_download",this);
	else {
		QFileInfo f(filename);
		if (!f.isAbsolute())
			filename = Settings::getS("TempDir")+"/"+filename;

		file = new QFile(filename, this);
	}

	Logger::log(tr("[INF] Lade \"%1\" zu \"%2\"").arg(url).arg(file->fileName()));
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
	
	progressDlg->setWindowTitle(tr("Lade ")+info.fileName());
	progressDlg->setLabelText(tr("Lade ")+url);
	progressDlg->setMaximum(0);
	progressDlg->setValue(0);
	progressDlg->show();
	
	aborting = false;
	if (!file) return false;
	if (!file->open(QIODevice::WriteOnly))
	{
		
		QMessageBox::critical(this, tr("Fehler beim Öffnen"),
				QString(tr("Konnte die temporäre Datei (%1) nicht öffnen:\n%2"))
				.arg(file->fileName()).arg(file->errorString()));
		return false;
	}
	
	this->request = loader->get(urlD.path(), file);
	
	return true;
}

/**
 * \brief reads the response header from the server
 * \author Peter Grasch
 * @param QHttpResponseHeader header
 * The header we recieved
 */
void QuickDownloader::readResponse(const QHttpResponseHeader header)
{
	Logger::log(tr("[INF] Erhaltene HTTP Antwort: \"")+QString::number(header.statusCode())+"\"");
	if (header.statusCode() != 200) {
		QMessageBox::information(this, tr("HTTP"),
					 tr("Download fehlgeschlagen: %1.")
							 .arg(header.reasonPhrase()));
		Logger::log(tr("[INF] Download fehlgeschlagen: %1.").arg(header.reasonPhrase()));
		aborting = true;
		progressDlg->close();
		loader->abort();
		return;
	}
}

/**
 * \brief Cancels the current upload and returns
 * \author Peter Grasch
 */
void QuickDownloader::cancelDownload()
{
	Logger::log(tr("[INF] Download zurückgesetzt"));
	aborting = true;
	progressDlg->close();
	loader->abort();
}

/**
 * \brief Updates the progressbar and emits the current progress
 * @param int now
 * How many bytes have we already recieved?
 * @param int max
 * How many bytes are there to retrieve?
 */
void QuickDownloader::dataRecieved(int now, int max)
{
	if (aborting) return;
	
	progressDlg->setMaximum(max);
	progressDlg->setValue(now);
	emit progress(now, max);
}

/**
 * \brief Emits signals like downloadFinished(...), errorOccured(...) and aborted(); cleans up
 * @param int id
 * ID of the finished request
 * @param bool error
 * Was there an error?
 */
void QuickDownloader::requestFinished(int id, bool error)
{
	if (error) {
		Logger::log(tr("[ERR] Fehler beim Download aufgetreten: \"")+loader->errorString()+"\"");
		emit errorOccured(loader->errorString());
	}
	
	if ((id == this->request) && (aborting == false)) {
		Logger::log(tr("[INF] Download abgeschlossen: \"")+file->fileName()+"\"");
		if (file)
			file->close();
		progressDlg->hide();
		emit downloadFinished(file->fileName());
	}
	if (aborting)
	{
		Logger::log(tr("[INF] Download abgebrochen: \"")+file->fileName()+"\"");
		if (file) {
			file->close();
			file->remove();
		}
		progressDlg->hide();
		emit aborted();
	}
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
QuickDownloader::~QuickDownloader()
{
	if (loader)
		loader->deleteLater();

	if (file) this->destroyFile();
	
	if (progressDlg)
		progressDlg->deleteLater();
}


