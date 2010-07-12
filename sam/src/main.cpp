/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>
#include "../../version.h"

static const char description[] =
I18N_NOOP("An acoustic model modeller");

int main(int argc, char **argv)
{
  KAboutData about("sam", 0, ki18n("sam"), simon_version, ki18n(description),
    KAboutData::License_GPL, ki18n("(C) 2009 Peter Grasch"), KLocalizedString(), 0, "grasch@simon-listens.org");
  about.addAuthor( ki18n("Peter Grasch"), KLocalizedString(), "grasch@simon-listens.org" );
  KCmdLineArgs::init(argc, argv, &about);

  KApplication app;
  SamView *widget = new SamView(0,0);
  widget->show();

  return app.exec();
}

