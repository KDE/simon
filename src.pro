unix {
    HEADERS += runlinuxbackend.h xevents.h desktopreader.h \
 dbusbackend.h \
 kdeprogrammanager.h 
    SOURCES += xevents.cpp runlinuxbackend.cpp desktopreader.cpp \
 dbusbackend.cpp \
 kdeprogrammanager.cpp
    CONFIG += x11 qdbus
    DEFINES += __LINUX_ALSA__
    INCLUDEPATH += /usr/include/alsa \
		/usr/X11R6/include
    LIBS += \
	-L/usr/X11R6/lib \
	-L/usr/lib/alsa-lib \
	-lX11 \
	-lXtst \
	-lasound
    TARGET = simon
}

win32 {
    HEADERS += runwindowsbackend.h registrymanager.h windowsevents.h windowsprogrammanager.h windowsresourcehandler.h msaabackend.h
    SOURCES += runwindowsbackend.cpp registrymanager.cpp windowsevents.cpp windowsprogrammanager.cpp  windowsresourcehandler.cpp msaabackend.cpp

    DEFINES += __WINDOWS_DS__
    LIBS += -ldsound -lwinmm -lole32 -oleacc -atls
    TARGET = ../simon
}

TRANSLATIONS += simon_en.ts simon_tob.ts

OBJECTS_DIR = buildtmp
MOC_DIR = buildtmp

FORMS += ui/main.ui \
         ui/wordlist.ui \
         ui/rundialog.ui \
	 ui/commandsettingsdlg.ui \
         ui/trainmain.ui \
         ui/importdict.ui \
         ui/generalsettingsdlg.ui \
         ui/soundsettingsdlg.ui \
	 ui/logmanagerdlg.ui \
         ui/externalprogramms.ui \
         ui/networksettingsdlg.ui \
 ui/shortcutsettingsdlg.ui \
 ui/desktopgridconfigurationdlg.ui \
 ui/modelsettingsdlg.ui \
 ui/recwidget.ui \
 ui/removeword.ui \
 ui/resolvewordpage.ui \
 ui/grammarsettingsdlg.ui \
 ui/selectfilestoreadpage.ui \
 ui/importgrammarworkingpage.ui \
 ui/systemviewdlg.ui \
 ui/passwordsettingsdlg.ui \
 ui/mergeterminalsselectpage.ui \
 ui/mergeterminalsworkingpage.ui \
 ui/selectlanguagemodelsource.ui \
 ui/renameterminalselectparameterspage.ui \
 ui/renameterminalworkingpage.ui \
 ui/configuretriggerspage.ui \
 ui/definiedefaultvaluesdlg.ui \
 ui/importdictlexiconpage.ui \
 ui/importgrammardescriptionpage.ui \
 ui/internetextensiondlg.ui \
 ui/atsettings.ui \
 ui/importtrainingtextremotepage.ui \
 ui/importtrainingtextworkingpage.ui \
 ui/importtrainingtextselectsourcepage.ui \
 ui/importtrainingtextlocalpage.ui


HEADERS += simonview.h \
           addwordview.h \
           simoncontrol.h \
           simoninfo.h \
           wordlistview.h \
	   screengrid.h \
           trainingview.h \
           juliuscontrol.h \
           wordlistmanager.h \
           command.h \
           modelmanager.h \
           runcommand.h \
           trainingmanager.h \
           trainingtext.h \
           word.h \
           trayiconmanager.h \
	   RtAudio.h \
	   RtError.h \
           osd.h \
	   dragtablewidget.h \
	   droplistwidget.h \
           runapplicationview.h \
           runbackend.h \
           xmlreader.h \
           eventhandler.h \
           coreevents.h \
           wav.h \
           xmlcommand.h \
           xmltrainingtext.h \
           wavrecorder.h \
           wavplayer.h \
	   sounddevice.h \
           soundcontrol.h \
           importdictview.h \
           importdict.h \
           wiktionarydict.h \
           dict.h \
           xmldomreader.h \
           xmlsaxreader.h \
           importtrainingtexts.h \
	   recwidget.h \
 quickdownloader.h \
 xmltrainingtextlist.h \
 importtrainingtextremotepage.h \
 importtrainingtextlocalpage.h \
 importtrainingtextselectsourcepage.h \
 importtrainingtextworkingpage.h \
 logger.h \
importtrainingdirectory.h \
importtrainingdirectoryintropage.h \
importtrainingdirectoryworkingpage.h \
           addwordintropage.h \
           addwordrecordpage.h \
           importbomppage.h \
           importdictworkingpage.h \
           importdictwiktionarypage.h \
           importdictselectsourcepage.h \
           bunzip.h \
           bompdict.h \
           quickunpacker.h \
           logentry.h \
           logmanager.h \
           inlinewidgetview.h \
           inlinewidget.h \
           systemview.h \
           systemwidget.h \
           generalsettings.h \
           soundsettings.h \
           settings.h \
           logview.h \
           externalprogrammanager.h \
           networksettings.h \
           importprogramwizard.h \
           selectprogrampage.h \
           configureprogrampage.h \
           importprogrampage.h \
           guievents.h \
	   simonwidget.h \
	   simonmainwindow.h \
	   simoncombobox.h \
	   simonspinbox.h \
	   simonlistwidget.h \
	   commandsettings.h \
	   shortcut.h \
	   shortcutcontrol.h \
	   shortcutxmlreader.h \
	   shortcutsettings.h \
	   selectshortcutbutton.h \
	   key.h \
	   simontablewidget.h \
	   simontabwidget.h \
	   simongroupbox.h \
	   simoncalendarwidget.h \
	   simonslider.h \
 program.h \
 programcategory.h \
 programmanager.h \
 categoryxmlreader.h \
 inireader.h \
 importplacewizard.h \
 importplacepage.h \
 localplacepage.h \
 introplacepage.h \
 passworddlg.h \
 remoteplacepage.h \
 desktopgridconfiguration.h \
 modelsettings.h \
 simonlineedit.h \
 deleteworddialog.h \
 icondialog.h \
 iconbutton.h \
 addwordresolvepage.h \
 grammarmanager.h \
 grammarsettings.h \
 importgrammarwizard.h \
 importgrammarselectfilespage.h \
 importgrammarworkingpage.h \
 importgrammar.h \
 passwordsettings.h \
 mergeterminalswizard.h \
 mergeterminalsselectterminalspage.h \
 mergeterminalsworkingpage.h \
 mergeterminals.h \
 firstrunwizard.h \
 systemwidgetpage.h \
 selectlanguagemodelsourcepage.h \
 firstrunimportdictselectsourcepage.h \
 firstrunmodelsettings.h \
 firstrunimportbomppage.h \
 firstrunimportgrammarworkingpage.h \
 firstruncreatedictionarypage.h \
 renameterminalwizard.h \
 renameterminalselectparameterspage.h \
 renameterminalworkingpage.h \
 renameterminal.h \
 configuretriggerspage.h \
 definedefaultvaluespage.h \
 importlexiconpage.h \
 lexicondict.h \
 firstrunimportdictwiktionarypage.h \
 firstrunimportgrammardescriptionpage.h \
 internetextensionsettings.h \
 postprocessing.h \
 atbackend.h \
 atwatcher.h \
 atobject.h \
 ato.h \
 atsettings.h  \
 compression.h \
 filesystemencoder.h \
 importtrainingdata.h



SOURCES += main.cpp \
           simonview.cpp \
	   RtAudio.cpp \
           addwordview.cpp \
           simoncontrol.cpp \
           simoninfo.cpp \
           wordlistview.cpp \
           trainingview.cpp \
           juliuscontrol.cpp \
           wordlistmanager.cpp \
           modelmanager.cpp \
           runcommand.cpp \
           trainingmanager.cpp \
           trainingtext.cpp \
           trayiconmanager.cpp \
           osd.cpp \
           runapplicationview.cpp \
           eventhandler.cpp \
           screengrid.cpp \
           wav.cpp \
		xmlreader.cpp \
           xmlcommand.cpp \
           xmltrainingtext.cpp \
           wavrecorder.cpp \
           wavplayer.cpp \
           soundcontrol.cpp \
           importdictview.cpp \
           importdict.cpp \
           wiktionarydict.cpp \
           dict.cpp \
           xmldomreader.cpp \
           xmlsaxreader.cpp \
           importtrainingtexts.cpp \
	   recwidget.cpp \
 quickdownloader.cpp \
 xmltrainingtextlist.cpp \
 importtrainingtextremotepage.cpp \
 importtrainingtextlocalpage.cpp \
 importtrainingtextselectsourcepage.cpp \
 importtrainingtextworkingpage.cpp \
 importtrainingdirectory.cpp \
 importtrainingdirectoryintropage.cpp \
 importtrainingdirectoryworkingpage.cpp \
           addwordintropage.cpp \
           addwordrecordpage.cpp \
           importbomppage.cpp \
           importdictworkingpage.cpp \
           importdictwiktionarypage.cpp \
           importdictselectsourcepage.cpp \
           bunzip.cpp \
           bompdict.cpp \
           quickunpacker.cpp \
           logentry.cpp \
           logmanager.cpp \
           inlinewidgetview.cpp \
           inlinewidget.cpp \
           systemview.cpp \
           systemwidget.cpp \
	   dragtablewidget.cpp \
	   droplistwidget.cpp \
           generalsettings.cpp \
           soundsettings.cpp \
           settings.cpp \
           logger.cpp \
           logview.cpp \
           guievents.cpp \
           externalprogrammanager.cpp \
           networksettings.cpp \
           importprogramwizard.cpp \
           selectprogrampage.cpp \
           configureprogrampage.cpp \
           importprogrampage.cpp \
	   commandsettings.cpp \
	   simonmainwindow.cpp \
	   simonwidget.cpp \
	   simoncombobox.cpp \
	   simonspinbox.cpp \
	   simonlistwidget.cpp \
	   shortcutcontrol.cpp \
	   shortcutxmlreader.cpp \
	   shortcutsettings.cpp \
	   selectshortcutbutton.cpp \
	   shortcut.cpp \
	   simontablewidget.cpp \
	   simontabwidget.cpp \
	   simongroupbox.cpp \
	   simoncalendarwidget.cpp \
	   simonslider.cpp \
 programmanager.cpp \
 categoryxmlreader.cpp \
 introplacepage.cpp \
 configureplacepage.cpp \
 passworddlg.cpp \
 inireader.cpp \
  importplacewizard.cpp \
 importplacepage.cpp \
 localplacepage.cpp \
 remoteplacepage.cpp \
 desktopgridconfiguration.cpp \
 modelsettings.cpp \
 simonlineedit.cpp \
 deleteworddialog.cpp \
 icondialog.cpp \
 iconbutton.cpp \
 addwordresolvepage.cpp \
 grammarmanager.cpp  \
 grammarsettings.cpp \
 importgrammarwizard.cpp \
 importgrammarselectfilespage.cpp \
 importgrammarworkingpage.cpp \
 importgrammar.cpp \
 passwordsettings.cpp \
 mergeterminalswizard.cpp \
 mergeterminalsselectterminalspage.cpp \
 mergeterminalsworkingpage.cpp \
 mergeterminals.cpp \
 firstrunwizard.cpp \
 systemwidgetpage.cpp \
 selectlanguagemodelsourcepage.cpp \
 firstrunmodelsettings.cpp \
 firstrunimportgrammarworkingpage.cpp \
 firstruncreatedictionarypage.cpp \
 renameterminalwizard.cpp \
 renameterminalselectparameterspage.cpp \
 renameterminalworkingpage.cpp \
 renameterminal.cpp \
 configuretriggerspage.cpp \
 definedefaultvaluespage.cpp \
 importlexiconpage.cpp \
 lexicondict.cpp \
 firstrunimportdictwiktionarypage.cpp \
 firstrunimportdictselectsourcepage.cpp \
 firstrunimportgrammardescriptionpage.cpp \
 internetextensionsettings.cpp \
 postprocessing.cpp \
 atwatcher.cpp \
 atobject.cpp \
 atsettings.cpp \
 filesystemencoder.cpp \
 importtrainingdata.cpp




QT += network \
xml

QT += ssl


TEMPLATE = app

CONFIG += qt \
debug \
warn_on \
thread \
console

RESOURCES += banners.qrc others.qrc action_icons.qrc application_icons.qrc category_icons.qrc other_icons.qrc
