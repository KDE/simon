/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "simonview.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <QTranslator>
#include <QLocale>


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	QString locale = QLocale::system().name().left(2);

	QTranslator translator;
	translator.load(QString("simon_%1").arg(locale));
	app.installTranslator(&translator);

	SimonView *pv = new SimonView();
	#ifdef __WIN32
	app.setStyle(new QPlastiqueStyle());
	#endif
	pv->show();
	return app.exec();
}
