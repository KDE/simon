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
#include <QProgressDialog>
#include <QObject>
#include <QMessageBox>
#include <QString>
#include "logger.h"

QuickUnpacker::QuickUnpacker(QWidget* parent): QWidget(parent)
{
}

void QuickUnpacker::unpack(QString path)
{
	prog = new QProgressDialog(tr("Extrahiere")+" "+path, tr("Abbrechen"), 0, 0);
	prog->show();
	
	Logger::log(tr("Extrahiere BZIP2 komprimierte Datei ")+path);
	bunzip = new Bunzip(this);
	connect(bunzip, SIGNAL(progress(int)), this, SLOT(setProgress(int)));
	connect(bunzip, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
	connect(bunzip, SIGNAL(extractionFinished(QString)), this, SLOT(unpacked(QString)));
	connect(prog, SIGNAL(canceled()), this, SLOT(cancel()));
	connect(bunzip, SIGNAL(canceled()), prog, SLOT(cancel()));
	connect(bunzip, SIGNAL(canceled()), this, SIGNAL(canceled()));
	bunzip->extract(path);
	
}

void QuickUnpacker::unpacked(QString to)
{
	setStatus(tr("BZIP2 Datei erfolgreich nach %1 extrahiert").arg(to));
	prog->done(0);
	emit unpackedTo(to);
}

void QuickUnpacker::setStatus(QString status)
{
	Logger::log(status);
	prog->setLabelText(status);
}

void QuickUnpacker::cancel()
{
	emit status(tr("Abbrechen..."));
	bunzip->cancel();
}

void QuickUnpacker::setProgress(int currentProg)
{
	emit this->progress(currentProg);
}

void QuickUnpacker::errorOccured(QString err)
{
	Logger::log(tr("[ERR] ")+err);
	QMessageBox::critical(this, tr("Fehler beim Entpacken"), tr("Beim Entpacken ist ein Fehler aufgetreten: ")+err);
	emit error(err);
}


QuickUnpacker::~QuickUnpacker()
{
	
}
