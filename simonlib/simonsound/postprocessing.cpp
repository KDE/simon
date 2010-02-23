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
#include <QFile>
#include <KProgressDialog>
#include <QCoreApplication>
#include <QObject>
#include <KLocalizedString>
#include <KLocale>
#include "soundconfig.h"

PostProcessing::PostProcessing(QObject *parent) : QObject(parent)
{
}

/**
 * \brief Takes a input and output filename, applies the postprocessing chain on the input and saves it
 * @param in The input filename
 * @param out Where to save it to
 * @return success
 */
bool PostProcessing::process(const QString& in, const QString& out, bool deleteIn, bool silent)
{
	KProgressDialog *progDialog=NULL;
	if (!silent) {
		progDialog = new KProgressDialog(0, i18n("Post-Processing"), 
				i18n("Filter a being applied..."));
	}
	if (QFile::exists(out) && (!QFile::remove(out)))
	{
		emit error(i18n("Couldn't overwrite %1.\n\nPlease check if you have the needed permissons.", out));
		return false;
	}
	
 	QStringList filters = SoundConfiguration::processingFilters();
	QString filter;
	if (!silent) {
		progDialog->progressBar()->setMaximum(filter.count()+1);
		QCoreApplication::processEvents();
	}
	for (int j=0; j < filters.count(); j++)
	{
		QString execStr = filters.at(j);
		execStr.replace("%1", in);
		execStr.replace("%2", out);
// 		execStr.replace("\%3", QString::number(CoreConfiguration::modelSampleRate()));
// 		execStr.replace("\%4", QString::number(CoreConfiguration::modelChannels()));
		int ret = QProcess::execute(execStr);
		if (ret)
		{
			//something went wrong
			emit error(i18n("Couldn't process \"%1\" to \"%2\". Please check if the command:\n\"%4\". (Return value: %3)", in, out, ret, execStr));
			return NULL;
		}
		if (!silent) {
			progDialog->progressBar()->setValue(j+1);
			QCoreApplication::processEvents();
		}
	}

	if (!QFile::exists(out)) { //if there are no filters or they don't copy it to the output
		if (!QFile::copy(in, out))
		{
			emit error(i18n("Couldn't copy %1 to %2. Please check if you have all the needed permissions.", in, out));
			return false;
		}
	}

	if (deleteIn) {
		if (!QFile::remove(in)) {
			emit error(i18n("Couldn't remove %1", in));
		}
	}

	if (!silent) {
		QCoreApplication::processEvents();
		progDialog->progressBar()->setValue(progDialog->progressBar()->maximum());
		progDialog->close();
		progDialog->deleteLater();
	}

	return true;
}


PostProcessing::~PostProcessing()
{
}


