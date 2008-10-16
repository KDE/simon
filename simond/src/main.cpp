#include "simond.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>

static const char description[] =
    I18N_NOOP("A KDE 4 Application");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
    KAboutData about("simond", 0, ki18n("simond"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2007 Peter Grasch"), KLocalizedString(), 0, "grasch@simon-listens.org");
    about.addAuthor( ki18n("Peter Grasch"), KLocalizedString(), "grasch@simon-listens.org" );
    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("+[URL]", ki18n( "Document to open" ));
    KCmdLineArgs::addCmdLineOptions(options);
    KApplication app;

    simond *widget = new simond;

    // see if we are starting with session management
    if (app.isSessionRestored())
    {
        RESTORE(simond);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
        if (args->count() == 0)
        {
            //simond *widget = new simond;
            widget->show();
        }
        else
        {
            int i = 0;
            for (; i < args->count(); i++)
            {
                //simond *widget = new simond;
                widget->show();
            }
        }
        args->clear();
    }

    return app.exec();
}
