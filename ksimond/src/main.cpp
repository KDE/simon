/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ksimondview.h"

#include <KCoreAddons/kaboutdata.h>

#include <QApplication>
#include <KAboutData>
#include <KLocalizedString>
#include <QCommandLineParser>
#include "version.h"

static const char description[] =
I18N_NOOP("A KDE 4 frontend for Simond");

int main(int argc, char **argv)
{
  KAboutData about("ksimond", i18n("KSimond"), simon_version,
                   i18n(description), KAboutLicense::GPL,
                   i18n("(C) 2008 Peter Grasch"), QString(), QString(), "peter.grasch@bedahr.org");
  about.addAuthor( i18n("Peter Grasch"), QString(), "peter.grasch@bedahr.org" );
  QApplication app(argc, argv);
  QCommandLineParser parser;
  parser.addVersionOption();
  parser.addHelpOption();

  //QT5TODO: KCommandLineParser script botched. Figure out what this was doing before and replace it.
  //PORTING SCRIPT: adapt aboutdata variable if necessary
  // aboutData.setupCommandLine(&parser);
  parser.process(app);
  // aboutData.processCommandLine(&parser);
  KAboutData::setApplicationData(about);

  QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath()+"/../plugins");
  app.setQuitOnLastWindowClosed(false);
  KSimondView *widget = new KSimondView();
  //widget->show();

  int ret = app.exec();
  delete widget;
  return ret;
}

