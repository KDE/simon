//
// C++ Implementation: quickunpacker
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "quickunpacker.h"
#include <KProgressDialog>
#include <QObject>
#include <KMessageBox>
#include <QString>
#include <KLocalizedString>
#include "../Logging/logger.h"
#include "bunzip.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the Widget
 */
QuickUnpacker::QuickUnpacker(QWidget* parent): QWidget(parent)
{
	compression = 0;
}

/**
 * \brief Unpacks the given file
 * \author Peter Grasch
 * @param path the path of the file to unpack
 */
void QuickUnpacker::unpack(QString path)
{
	prog = new KProgressDialog(this, i18n("Extrahiere..."), i18n("Extrahiere %1...", path));
	prog->progressBar()->setValue(0);
	prog->progressBar()->setMaximum(0);
	prog->show();
	
	Logger::log(i18n("Extrahiere BZIP2 komprimierte Datei ")+path);

	if (compression) compression->deleteLater();

	compression = new Bunzip(this);
	connect(compression, SIGNAL(progress(int)), this, SLOT(setProgress(int)));
	connect(compression, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
	connect(compression, SIGNAL(extractionFinished(QString)), this, SLOT(unpacked(QString)));
	connect(prog, SIGNAL(canceled()), this, SLOT(cancel()));
	connect(compression, SIGNAL(canceled()), this, SIGNAL(canceled()));
	compression->extract(path);
	
}

/**
 * \brief Reacts on finishing the unpack-process; emits unapckedTo(path-to-new-file)
 * \author Peter Grasch
 * @param to the path we unpacked to
 */
void QuickUnpacker::unpacked(QString to)
{
	setStatus(i18n("BZIP2 Datei erfolgreich nach %1 extrahiert").arg(to));
	prog->done(0);
	emit unpackedTo(to);
}

/**
 * \brief Sets the Status to the given status
 * \author Peter Grasch
 * @param status status
 */
void QuickUnpacker::setStatus(QString status)
{
	Logger::log(i18n("[INF] ")+status);
	prog->setLabelText(status);
}

/**
 * \brief Cancels the unpacking-process
 * \author Peter Grasch
 */
void QuickUnpacker::cancel()
{
	emit status(i18n("Abbrechen..."));
	compression->cancel();
}

/**
 * \brief Sets the progress to the given progress
 * \author Peter Grasch
 * @param currentProg progress
 */
void QuickUnpacker::setProgress(int currentProg)
{
	emit this->progress(currentProg);
}

/**
 * \brief An error occurred - we react on it: Shows a messagebox and emits error(the-error)
 * \author Peter Grasch
 * @param err The error
 */
void QuickUnpacker::errorOccured(QString err)
{
	Logger::log(i18n("[ERR] Fehler beim Entpacken"));
	KMessageBox::error(this, i18n("Beim Entpacken ist ein Fehler aufgetreten: %1").arg(err));
	emit error(err);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
QuickUnpacker::~QuickUnpacker()
{
    prog->deleteLater();
    compression->deleteLater();
}
