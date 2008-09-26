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
#include <KUniqueApplication>
#include <KAboutData>
#include <KCmdLineArgs>


int main(int argc, char *argv[])
{
	KAboutData aboutData( "simon", "simon",
			ki18n("simon"), "0.1-alpha-2.99",
			      ki18n("<html><head /><body>\
					      <h2>Entwicklung und Vertrieb</h2>\
					      <p>Entwickelt und Vertrieben vom gemeinnützigen Verein <a href=\"http://simon-listens.org\">simon listens e.V.</a> in Kooperation mit <a href=\"http://cyber-byte.at\">Cyber-Byte EDV Services</a>.</p>\
					      <h3>Dank an</h3> \
					      <ul><li>Franz Stieger<li>Mathias Stieger<li>Phillip Theussl<li>Moacyr Prado<li>Stieger Michael</ul> \
					      </body></html>"),
			KAboutData::License_GPL,
			ki18n("Copyright (c) 2008 Peter Grasch, hillip Goriup, Tschernegg Susanne, Bettina Sturmann, Martin Gigerl") );
	
	KCmdLineArgs::init(argc, argv, &aboutData);
	KUniqueApplication app;	
	app.setWindowIcon(KIcon("simon"));
	app.addLibraryPath(app.applicationDirPath()+"/plugins");
// 	QString locale = QLocale::system().name().left(2);

// 	QTranslator translator;

// 	translator.load(QString("simon_%1", locale));

// 	app.installTranslator(&translator);

	SimonView *pv = new SimonView();
	pv->show();
	return app.exec();
}
