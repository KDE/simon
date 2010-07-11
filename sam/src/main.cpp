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
