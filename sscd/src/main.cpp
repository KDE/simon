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

#include "sscdcontrol.h"

#include <QCoreApplication>

#include <QApplication>
#include "version.h"
#include "basedirectory.h"
#include <QStringList>

int main(int argc, char **argv)
{
  QCoreApplication::addLibraryPath(SSCD_BASE_DIRECTORY);

  QCoreApplication app(argc,argv);

  if (app.arguments().contains("-h") || app.arguments().contains("--help")) {
    qWarning() << "Run without arguments to start a full server (to be used with SSC).";
    qWarning() << "Use the -l Argument (or --locked) to only allow functions needed to submit samples with simon.";
    return 0;
  }
  SSCDControl *control = new SSCDControl();
  if (!control->init())
    return 2;

  app.exec();
  delete control;
  return 0;
}
