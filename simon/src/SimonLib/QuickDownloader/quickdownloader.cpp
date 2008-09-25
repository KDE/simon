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


#include "quickdownloader.h"
#include <QTemporaryFile>
#include <QFile>
#include <QVariant>
#include <KProgressDialog>
#include <QHttp>
#include <QUrl>
#include <KMessageBox>
#include <QFileInfo>
#include <KLocalizedString>
#include "../Settings/settings.h"
#include "../Logging/logger.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param QWidget *parent
 * The parent of the Widget - is passed on to the QWidget constructor
 */
QuickDownloader::QuickDownloader(QWidget *parent) : QWidget(parent)
{
	this->loader = new QHttp();
	this->progressDlg = new KProgressDialog(this);
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

	Logger::log(i18n("[INF] Lade \"%1\" zu \"%2\"", url, file->fileName()));
	if (!loader || !progressDlg) return false;
	
	QUrl urlD = QUrl(url);
	
	QHttp::ConnectionMode mode = urlD.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	
	if (!urlD.userName().isEmpty())
		loader->setUser(urlD.userName(), urlD.password());
	
	
	connect (loader, SIGNAL(requestFinished(int, bool)), this, SLOT(requestFinished(int, bool)));
	connect(loader, SIGNAL(dataReadProgress(int, int)), this, SLOT(dataReceived(int, int)));
	connect(progressDlg, SIGNAL(canceled()), this, SLOT(cancelDownload()));
	connect(loader, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
		this, SLOT(readResponse(const QHttpResponseHeader &)));
	
	
	loader->setHost(urlD.host(), mode, urlD.port() == -1 ? 0 : urlD.port());;
	QFileInfo info = QFileInfo(urlD.path());
	
	progressDlg->setWindowTitle(i18n("Lade ")+info.fileName());
	progressDlg->setLabelText(i18n("Lade ")+url);
	progressDlg->progressBar()->setMaximum(0);
	progressDlg->progressBar()->setValue(0);
	progressDlg->show();
	
	aborting = false;
	if (!file) return false;
	if (!file->open(QIODevice::WriteOnly))
	{
		
		KMessageBox::error(this,
				i18n("Konnte die temporäre Datei (%1) nicht öffnen:\n%2", file->fileName(), file->errorString()));
		return false;
	}
	
	this->request = loader->get(urlD.path(), file);
	
	return true;
}

/**
 * \brief reads the response header from the server
 * \author Peter Grasch
 * @param QHttpResponseHeader header
 * The header we received
 */
void QuickDownloader::readResponse(const QHttpResponseHeader header)
{
	Logger::log(i18n("[INF] Erhaltene HTTP Antwort: \"")+QString::number(header.statusCode())+"\"");
	if (header.statusCode() != 200) {
		KMessageBox::information(this, i18n("Download fehlgeschlagen: %1.", header.reasonPhrase()));
		Logger::log(i18n("[INF] Download fehlgeschlagen: %1.", header.reasonPhrase()));
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
	Logger::log(i18n("[INF] Download zurückgesetzt"));
	aborting = true;
	progressDlg->close();
	loader->abort();
}

/**
 * \brief Updates the progressbar and emits the current progress
 * @param int now
 * How many bytes have we already received?
 * @param int max
 * How many bytes are there to retrieve?
 */
void QuickDownloader::dataReceived(int now, int max)
{
	if (aborting) return;
	
	progressDlg->progressBar()->setMaximum(max);
	progressDlg->progressBar()->setValue(now);
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
		Logger::log(i18n("[ERR] Fehler beim Download aufgetreten: \"")+loader->errorString()+"\"");
		emit errorOccured(loader->errorString());
	}
	
	if ((id == this->request) && (aborting == false)) {
		Logger::log(i18n("[INF] Download abgeschlossen: \"")+file->fileName()+"\"");
		if (file)
			file->close();
		progressDlg->hide();
		emit downloadFinished(file->fileName());
	}
	if (aborting)
	{
		Logger::log(i18n("[INF] Download abgebrochen: \"")+file->fileName()+"\"");
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


