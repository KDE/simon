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

#include "simondcontrol.h"

#include <QCoreApplication>

#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KLocale>
#include <KGlobal>
#include <KComponentData>
#include <QApplication>

static const char description[] =
    I18N_NOOP("The simon recognition daemon");

static const char version[] = "0.2-alpha-2";

int main(int argc, char **argv)
{
	KAboutData about("simond", 0, ki18n("simond"), version, ki18n(description),
			KAboutData::License_GPL, ki18n("(C) 2008 Peter Grasch"), KLocalizedString(), 0, "grasch@simon-listens.org");
	about.addAuthor( ki18n("Peter Grasch"), KLocalizedString(), "grasch@simon-listens.org" );

	KCmdLineArgs::init(argc, argv, &about);

	KGlobal::setActiveComponent(KComponentData(about));

	QCoreApplication app(argc,argv);

	SimondControl *control = new SimondControl();
	if (!control->init())
		return 2;

	app.exec();
	delete control;
	return 0;
}
