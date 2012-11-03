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
#include "version.h"

#include <KDebug>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

class SimonApplication :
#ifndef Q_OS_WIN32
public KUniqueApplication
#else
public KApplication
#endif
{
  public:
  #ifdef Q_WS_X11
    bool x11EventFilter ( XEvent * event ) {
      if (event->type == MappingNotify) {
        XMappingEvent *e = (XMappingEvent*) event;
        XRefreshKeyboardMapping(e);
        return true;
      }
      return false;
    }
  #endif

};

int main(int argc, char *argv[])
{
  KAboutData aboutData( "Simon", "Simon",
    ki18n("Simon"), simon_version,
    ki18n("<html><head /><body>"
          "<h2>Development and Distribution</h2>"
          "<p>Developed in part by the non-profit organization"
          " <a href=\"http://simon-listens.org\">Simon listens e.V.</a>"
          " in cooperation with <a href=\"http://cyber-byte.at\">Cyber-Byte EDV Services</a>.</p>"
          "<h3>Special Thanks To</h3>"
          "<ul><li>Franz Stieger</li><li>Mathias Stieger</li><li>Phillip Theussl</li><li>Moacyr Prado</li><li>Michael Stieger</li><li>Ralf Herzog</li></ul>"
          "</body></html>"),
          KAboutData::License_GPL,
          ki18n("Copyright (c) 2008-2012 Peter Grasch, Phillip Goriup, Tschernegg Susanne, Bettina Sturmann, Martin Gigerl, Adam Nash, Frederik Gladhorn, Patrick von Reth, Alessadro Buggin, Mario Strametz, Vladislav Sitalo, Yash Shah") );

  KCmdLineOptions options;
  options.add("nosplash", ki18n("Disable Simon's splash screen"));
  KCmdLineArgs::addCmdLineOptions(options);

  KCmdLineArgs::init(argc, argv, &aboutData);

  SimonApplication app;
  app.setWindowIcon(KIcon("simon"));
  app.addLibraryPath(app.applicationDirPath()+"/plugins");

  SimonView *pv = new SimonView();

  Q_UNUSED(pv);
  int ret= app.exec();
//   pv is deleted automatically
  return ret;
}
