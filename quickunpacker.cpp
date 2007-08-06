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
#include <QString>
#include "logger.h"

QuickUnpacker::QuickUnpacker(QObject* parent): QObject(parent)
{
}

void QuickUnpacker::unpack(QString path)
{
	prog = new QProgressDialog(tr("Extrahiere")+" "+path, tr("Abbrechen"), 0, 100);
	bunzip = new Bunzip(this);
	connect(bunzip, SIGNAL(progress(int)), this, SLOT(setProgress(int)));
	connect(bunzip, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
	bunzip->extract(path);
	
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
	prog->cancel();
	emit canceled();
}

void QuickUnpacker::setProgress(int currentProg)
{
	emit this->progress(currentProg);
}

void QuickUnpacker::errorOccured(QString err)
{
	Logger::log(tr("Fehler: ")+err);
	emit error(err);
}


QuickUnpacker::~QuickUnpacker()
{
	
}
