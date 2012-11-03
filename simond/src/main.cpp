/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include <KDebug>
#include <KApplication>
#include <QApplication>
#include "../../version.h"

static const char description[] =
I18N_NOOP("The Simon recognition daemon (powered by the Large Vocabulary Continuous Speech Recognition Engine Julius)");

int main(int argc, char **argv)
{
  KAboutData about("Simond", 0, ki18n("Simond"), simon_version, ki18n(description),
    KAboutData::License_GPL, ki18n("(C) 2008-2010 Peter Grasch\n\nJulius:\n"
	    "Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan\n"
 	    "Copyright (c) 1991-2009 Kawahara Lab., Kyoto University\n"
 	    "Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology\n"
 	    "Copyright (c) 2005-2009 Julius project team, Nagoya Institute of Technology\n"
		), KLocalizedString(), 0, "peter.grasch@bedahr.org");
  about.addAuthor( ki18n("Peter Grasch"), KLocalizedString(), "peter.grasch@bedahr.org" );

  KCmdLineArgs::init(argc, argv, &about);

  KGlobal::setActiveComponent(KComponentData(about));

  QCoreApplication app(argc,argv);

  KGlobal::locale();
  KGlobal::locale()->insertCatalog("simonlib");
  SimondControl *control = new SimondControl();
  if (!control->init())
    return 2;

  app.exec();
  delete control;
  return 0;
}
