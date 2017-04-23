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

#include "samview.h"

#include <kaboutdata.h>

#include <KDE/KLocalizedString>
#include <QApplication>
#include <K4AboutData>
#include <KLocalizedString>
#include <KCmdLineArgs>
#include <KCmdLineOptions>
#include <KDELibs4Support/kapplication.h>
#include "version.h"

static const char description[] =
I18N_NOOP("An acoustic model modeller");

int main(int argc, char **argv)
{
  K4AboutData about("sam", 0, ki18n("Sam"), simon_version, ki18n(description),
    K4AboutData::License_GPL, ki18n("(C) 2009 Peter Grasch"), KLocalizedString(), 0, "peter.grasch@bedahr.org");
  about.addAuthor( ki18n("Peter Grasch"), KLocalizedString(), "peter.grasch@bedahr.org" );

  KCmdLineOptions options;
  options.add("paths-from-simon");
  options.add("m <type>", ki18n("Select model type (\"sphinx\" or \"htk\")"), "");
  options.add("s", ki18n("Get configuration from Simon"), "");
  options.add("c", ki18n("Automatically start compiling the model"), "");
  options.add("t", ki18n("Automatically start testing the model(s)"), "");
  options.add("e <filename>", ki18n("Export test results to the given filename"), "");
  options.add("b", ki18n("Batch processing: Close after all other arguments have been processed"), "");
  options.add("l <filename>", ki18n("Store compilation log"), "");
  options.add("w", ki18n("Write opened file to disk when done (batch mode)."), "");

  options.add("+samfile", ki18n("If provided, this file will automatically be loaded"), "");
     
  KCmdLineArgs::addCmdLineOptions(options);
  KCmdLineArgs::init(argc, argv, &about);

     

  KApplication app;
  QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath()+"/../plugins");
  SamView *widget = new SamView(0,0);

  if (!KCmdLineArgs::parsedArgs()->isSet("b")) {
    widget->show();
#ifdef Q_OS_MAC
    widget->raise();
#endif
  }

  return app.exec();
}

