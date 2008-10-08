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


#include "postprocessing.h"
#include <QProcess>
#include <KMessageBox>
#include <QFile>
#include <KProgressDialog>
#include <QCoreApplication>
#include <QObject>
#include <KLocalizedString>
// #include "coreconfiguration.h"

PostProcessing::PostProcessing()
{
}

/**
 * \brief Takes a input and output filename, applies the postprocessing chain on the input and saves it
 * @param in The input filename
 * @param out Where to save it to
 * @return success
 */
bool PostProcessing::process(const QString& in, const QString& out, bool deleteIn)
{
	KProgressDialog *progDialog = new KProgressDialog(0, i18n("Nachbearbeitung"), i18n("Filter werden angewendet..."));
	if (QFile::exists(out) && (!QFile::remove(out)))
	{
		KMessageBox::error(0, i18n("Konnte %1 nicht überschreiben. Bitte überprüfen Sie, ob Sie die nötigen Rechte besitzen.", out));
		return false;
	}
	
 	QStringList filters;// = CoreConfiguration::processingFilters();
	QString filter;
	progDialog->progressBar()->setMaximum(filter.count()+1);
	QCoreApplication::processEvents();
	for (int j=0; j < filters.count(); j++)
	{
		QString execStr = filters.at(j);
		execStr.replace("\%1", in);
		execStr.replace("\%2", out);
// 		execStr.replace("\%3", QString::number(CoreConfiguration::modelSampleRate()));
// 		execStr.replace("\%4", QString::number(CoreConfiguration::modelChannels()));
		int ret = QProcess::execute(execStr);
		if (ret)
		{
			//something went wrong
			KMessageBox::error(0, i18n("Konnte %1 nicht nach %2 bearbeiten.\n\nBitte ueberpruefen Sie ob Sie das Programm, installiert haben, der Pfad in den Einstellungen richtig angegeben wurde und ob Sie all die nötigen Berechtigungen besitzen. (Rückgabewert %3) (Ausgefuehrtes Kommando: %4)", in, out, ret, execStr));
			return NULL;
		}
		progDialog->progressBar()->setValue(j+1);
		QCoreApplication::processEvents();
	}

	if (!QFile::exists(out)) //if there are no filters or they don't copy it to the output
	{
		if (!QFile::copy(in, out))
		{
			KMessageBox::error(0, i18n("Konnte %1 nicht nach %2 kopieren. Bitte überprüfen Sie ihre Berechtigungen auf den Zielort", in, out));
			return false;
		}
	}

	if (deleteIn)
		if (!QFile::remove(in))
		{
			KMessageBox::error(0, i18n("Konnte %1 nicht löschen", in));
		}

	QCoreApplication::processEvents();
	progDialog->progressBar()->setValue(progDialog->progressBar()->maximum());
	progDialog->close();
	progDialog->deleteLater();

	return true;
}


PostProcessing::~PostProcessing()
{
}


