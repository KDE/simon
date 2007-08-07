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
	if (filename.isEmpty())
		file = new QTemporaryFile("simon_tmp_download",this);
	else file = new QFile(filename, this);

	Logger::log("Loading \""+url+"\" to \""+file->fileName()+"\"");
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
	progressDlg->setMaximum(0);
	progressDlg->setValue(0);
	progressDlg->show();
	
	aborting = false;
	if (!file) return false;
	if (!file->open(QIODevice::WriteOnly))
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

/**
 * \brief reads the response header from the server
 * \author Peter Grasch
 * @param QHttpResponseHeader header
 * The header we recieved
 */
void QuickDownloader::readResponse(const QHttpResponseHeader header)
{
	Logger::log("Got HTTP response: \""+QString::number(header.statusCode())+"\"");
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

/**
 * \brief Cancels the current upload and returns
 * \author Peter Grasch
 */
void QuickDownloader::cancelDownload()
{
	Logger::log("Canceling download");
	aborting = true;
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
		Logger::log("Error occured: \""+loader->errorString()+"\"");
		emit errorOccured(loader->errorString());
	}
	
	if ((id == this->request) && (aborting == false)) {
		Logger::log("Download finished: \""+filename+"\"");
		if (file)
			file->close();
		progressDlg->hide();
		emit downloadFinished(filename);
	}
	if (aborting)
	{
		Logger::log("Download aborted: \""+filename+"\"");
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
	delete progressDlg;
	delete loader;
	delete file;
}


