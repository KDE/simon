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



#include "simonview.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <QTranslator>
#include <QLocale>
#ifndef Q_OS_WIN32
#include <KUniqueApplication>
#else
#include <KApplication>
#endif
#include <KAboutData>
#include <KCmdLineArgs>

int main(int argc, char *argv[])
{
	KAboutData aboutData( "simon", "simon",
			ki18n("simon"), "0.2rc1",
			      ki18n("<html><head /><body>\
					      <h2>Development and Distribution</h2>\
					      <p>Developed and distributed by the friendly society <a href=\"http://simon-listens.org\">simon listens e.V.</a> in cooperation with <a href=\"http://cyber-byte.at\">Cyber-Byte EDV Services</a>.</p>\
					      <h3>Special Thanks To</h3> \
					      <ul><li>Franz Stieger<li>Mathias Stieger<li>Phillip Theussl<li>Moacyr Prado<li>Michael Stieger<li>Ralf Herzog</ul> \
					      </body></html>"),
			KAboutData::License_GPL,
			ki18n("Copyright (c) 2008 Peter Grasch, Phillip Goriup, Tschernegg Susanne, Bettina Sturmann, Martin Gigerl") );
	
	KCmdLineArgs::init(argc, argv, &aboutData);

#ifndef Q_OS_WIN32
	KUniqueApplication app;
#else
	KApplication app;
#endif
	app.setWindowIcon(KIcon("simon"));
	app.addLibraryPath(app.applicationDirPath()+"/plugins");

	SimonView *pv = new SimonView();
	int ret= app.exec();
	delete pv;
	return ret;
}
