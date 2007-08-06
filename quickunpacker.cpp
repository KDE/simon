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
#include "bunzip.h"
#include "logger.h"

QuickUnpacker::QuickUnpacker(QObject* parent): QObject(parent)
{
}

void QuickUnpacker::unpack(QString path)
{
	prog = new QProgressDialog(tr("Extrahiere")+" "+path, tr("Abbrechen"), 0, 100);
	Bunzip *bunzip = new Bunzip(this);
	connect(bunzip, SIGNAL(progress(int)), this, SLOT(setProgress(int)));
	connect(bunzip, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
	bunzip->extract(path);
	
}

void QuickUnpacker::setStatus(QString status)
{
	prog->setLabelText(status);
}

void QuickUnpacker::setProgress(int currentProg)
{
	emit this->progress(progress);
}

void QuickUnpacker::errorOccured(QString error)
{
	
}


QuickUnpacker::~QuickUnpacker()
{
	
}


