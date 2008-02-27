//
// C++ Implementation: postprocessing
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "postprocessing.h"
#include "settings.h"
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <QProgressDialog>
#include <QObject>

PostProcessing::PostProcessing()
{
}

/**
 * \brief Takes a input and output filename, applies the postprocessing chain on the input and saves it
 * @param in The input filename
 * @param out Where to save it to
 * @return success
 */
bool PostProcessing::process(QString in, QString out, bool deleteIn)
{
	QProgressDialog *progDialog = new QProgressDialog(QObject::tr("Filter werden angewendet..."), QObject::tr("Abbrechen"), 0, 1);
	if (QFile::exists(out) && (!QFile::remove(out)))
	{
		QMessageBox::critical(0, QObject::tr("Fehler"), QObject::tr("Konnte %1 nicht überschreiben. Bitte überprüfen Sie, ob Sie die nötigen Rechte besitzen.").arg(out));
		return false;
	}
	
	QStringList filters = Settings::getS("Model/ProcessingFilters").split(" && ", QString::SkipEmptyParts);
	QString filter;
	progDialog->setMaximum(filter.count()+1);
	for (int j=0; j < filters.count(); j++)
	{
		progDialog->setValue(j+1);
		QString execStr = filters.at(j);
		execStr.replace("\%1", in);
		execStr.replace("\%2", out);
		execStr.replace("\%3", Settings::getS("Model/SampleRate"));
		execStr.replace("\%4", Settings::getS("Model/Channels"));
		int ret = QProcess::execute(execStr);
		if (ret)
		{
			//something went wrong
			QMessageBox::critical(0, QObject::tr("Fehler"), QObject::tr("Konnte %1 nicht nach %2 bearbeiten. Bitte ueberpruefen Sie ob Sie das Programm, installiert haben, der Pfad in den Einstellungen richtig angegeben wurde und ob Sie all die nötigen Berechtigungen besitzen. (Rückgabewert %3) (Ausgefuehrtes Kommando: %4)").arg(in).arg(out).arg(ret).arg(execStr));
			return NULL;
		}
	}

	if (!QFile::exists(out)) //if there are no filters or they don't copy it to the output
	{
		if (!QFile::copy(in, out))
		{
			QMessageBox::critical(0, QObject::tr("Fehler"), QObject::tr("Konnte %1 nicht nach %2 kopieren. Bitte überprüfen Sie ihre Berechtigungen auf den Zielort").arg(in).arg(out));
			return false;
		}
	}

	if (deleteIn)
		if (!QFile::remove(in))
		{
			QMessageBox::critical(0, QObject::tr("Fehler"), QObject::tr("Konnte %1 nicht löschen").arg(in));
		}

	progDialog->setValue(progDialog->maximum());
	progDialog->close();
	progDialog->deleteLater();

	return true;
}


PostProcessing::~PostProcessing()
{
}


