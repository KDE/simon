# "Installing" KDE files required for simon / simond / ksimond to the same location they are from
# Use this to bundle KDE / Qt / Portaudio with simon for (cpack generated) windows installer
IF(WIN32)
find_package(KDE4 REQUIRED)
ADD_CUSTOM_TARGET(package) #remove all
#set_target_properties(package PROPERTIES EXCLUDE_FROM_ALL)

install( FILES 
  "${KDE4_BIN_INSTALL_DIR}/aspell.exe"
  "${KDE4_BIN_INSTALL_DIR}/meinproc4.exe"
  "${KDE4_BIN_INSTALL_DIR}/bzip2.dll"
  "${KDE4_BIN_INSTALL_DIR}/portaudio.dll"
  "${KDE4_BIN_INSTALL_DIR}/portaudio_x86.dll"
  "${KDE4_BIN_INSTALL_DIR}/dbus-daemon.exe"
  "${KDE4_BIN_INSTALL_DIR}/dbus-launch.exe"
  "${KDE4_BIN_INSTALL_DIR}/iconv.dll"
  "${KDE4_BIN_INSTALL_DIR}/libattica.dll"
  "${KDE4_BIN_INSTALL_DIR}/intl.dll"
  "${KDE4_BIN_INSTALL_DIR}/jpeg62.dll"
  "${KDE4_BIN_INSTALL_DIR}/kbuildsycoca4.exe"
  "${KDE4_BIN_INSTALL_DIR}/khelpcenter.exe"
  "${KDE4_BIN_INSTALL_DIR}/kdebugdialog.exe"
  "${KDE4_BIN_INSTALL_DIR}/kded4.exe"
  "${KDE4_BIN_INSTALL_DIR}/kdeinit4.exe"
  "${KDE4_BIN_INSTALL_DIR}/kiconfinder.exe"
  "${KDE4_BIN_INSTALL_DIR}/kde4-config.exe"
  "${KDE4_BIN_INSTALL_DIR}/kioexec.exe"
  "${KDE4_BIN_INSTALL_DIR}/khotnewstuff-upload.exe"
  "${KDE4_BIN_INSTALL_DIR}/khotnewstuff4.exe"
  "${KDE4_BIN_INSTALL_DIR}/kioslave.exe"
  "${KDE4_BIN_INSTALL_DIR}/kio_http_cache_cleaner.exe"
  "${KDE4_BIN_INSTALL_DIR}/klauncher.exe"
  "${KDE4_BIN_INSTALL_DIR}/kmimetypefinder.exe"
  "${KDE4_BIN_INSTALL_DIR}/kuiserver.exe"
  "${KDE4_BIN_INSTALL_DIR}/libaspell-15.dll"
  "${KDE4_BIN_INSTALL_DIR}/libdbus-1.dll"
  "${KDE4_BIN_INSTALL_DIR}/libeay32.dll"
  "${KDE4_BIN_INSTALL_DIR}/libexpat.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libgettextpo-0.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libgettextsrc-0-17.dll"
  "${KDE4_BIN_INSTALL_DIR}/libiconv.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libiconv2.dll"
  "${KDE4_BIN_INSTALL_DIR}/libiconv-2.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libintl-2.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkdewin.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkdewin32.dll"
  "${KDE4_BIN_INSTALL_DIR}/libknewstuff3.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libintl3.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkdecore.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkdeui.dll"
  
  
  
  #"${KDE4_BIN_INSTALL_DIR}/wintab.dll"
  #"${KDE4_BIN_INSTALL_DIR}/wintab32.dll"
  #"${KDE4_BIN_INSTALL_DIR}/drkonqi.exe"
  #"${KDE4_BIN_INSTALL_DIR}/dwmapi.dll"
  "${KDE4_BIN_INSTALL_DIR}/kconf_update.exe"
  "${KDE4_BIN_INSTALL_DIR}/libkntlm.dll"
  
  
  "${KDE4_BIN_INSTALL_DIR}/libkde3support.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkhtml.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkjs.dll"
  "${KDE4_BIN_INSTALL_DIR}/libpcre.dll"
  "${KDE4_BIN_INSTALL_DIR}/libktexteditor.dll"
  "${KDE4_BIN_INSTALL_DIR}/giflib4.dll"
  "${KDE4_BIN_INSTALL_DIR}/libphonon.dll"
  
  "${KDE4_BIN_INSTALL_DIR}/libkdewin32.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkfile.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkio.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libkmime.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkparts.dll"
  #"${KDE4_BIN_INSTALL_DIR}/libkresources.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkutils.dll"
  "${KDE4_BIN_INSTALL_DIR}/libpng12.dll"
  "${KDE4_BIN_INSTALL_DIR}/libsolid.dll"
  "${KDE4_BIN_INSTALL_DIR}/libstreamanalyzer.dll"
  "${KDE4_BIN_INSTALL_DIR}/libstreams.dll"
  "${KDE4_BIN_INSTALL_DIR}/libxml2.dll"
  #"${KDE4_BIN_INSTALL_DIR}/mfc71.dll"
  "${KDE4_BIN_INSTALL_DIR}/mingwm10.dll"
  #"${KDE4_BIN_INSTALL_DIR}/mingw-pcrecpp.dll"
  #"${KDE4_BIN_INSTALL_DIR}/msvcp71.dll"
  "${KDE4_BIN_INSTALL_DIR}/libgcc_s_dw2-1.dll"
  #"${KDE4_BIN_INSTALL_DIR}/msvcr71.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtCore4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtDBus4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtGui4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtNetwork4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtMultimedia4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtSql4.dll"
  "${KDE4_BIN_INSTALL_DIR}/Qt3Support4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtSvg4.dll"
  "${KDE4_BIN_INSTALL_DIR}/QtXml4.dll"
  "${KDE4_BIN_INSTALL_DIR}/ssleay32.dll"
  "${KDE4_BIN_INSTALL_DIR}/update-mime-database.exe"
  "${KDE4_BIN_INSTALL_DIR}/zlib1.dll"
  "${KDE4_BIN_INSTALL_DIR}/libdbusmenu-qt.dll"
  "${KDE4_BIN_INSTALL_DIR}/libkcmutils.dll"
  "${KDE4_BIN_INSTALL_DIR}/libnepomuk.dll"
  "${KDE4_BIN_INSTALL_DIR}/libnepomukcommon.dll"
  "${KDE4_BIN_INSTALL_DIR}/libnepomukquery.dll"
  "${KDE4_BIN_INSTALL_DIR}/libnepomukutils.dll"
  "${KDE4_BIN_INSTALL_DIR}/libsoprano.dll"
  "${KDE4_BIN_INSTALL_DIR}/libsopranoclient.dll"
  "${KDE4_BIN_INSTALL_DIR}/libxslt.dll"
		DESTINATION  ${BIN_INSTALL_DIR} COMPONENT kdecore )
		
install (FILES  "${KDE4_LIB_INSTALL_DIR}/attica_kde.dll" 
		DESTINATION  ${BIN_INSTALL_DIR}/../lib COMPONENT kdecore)

FILE(GLOB kde4_plugins_binary "${KDE4_LIB_INSTALL_DIR}/kde4/*.dll")
install (FILES ${kde4_plugins_binary} DESTINATION  ${BIN_INSTALL_DIR}/../lib/kde4 COMPONENT kdecore)
		
# install ( FILES
  # "${KDE4_LIB_INSTALL_DIR}/kde4/kio_help.dll"
		# DESTINATION  ${BIN_INSTALL_DIR}/../lib/kde4 COMPONENT kdecore )
		
install ( FILES
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de.dat"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de_affix.dat"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de_AT.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de_AT-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de_DE.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de_DE-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de_phonet.dat"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/de-common.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/deutsch.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/german.alias"
  #"${KDE4_LIB_INSTALL_DIR}/aspell-0.60/american.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/american-w_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/american-wo_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-ise.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-ise-w_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-ise-wo_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-ize.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-ize-w_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-ize-wo_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-w_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/british-wo_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/canadian.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/canadian-w_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/canadian-wo_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en.dat"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_affix.dat"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_CA.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_CA-w_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_CA-w_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_CA-wo_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_CA-wo_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ise.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ise-w_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ise-w_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ise-wo_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ise-wo_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ize.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ize-w_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ize-w_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ize-wo_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-ize-wo_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-w_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_GB-wo_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_phonet.dat"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_US.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_US-w_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_US-w_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_US-wo_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en_US-wo_accents-only.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-common.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/english.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/english-variant_0.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/english-variant_1.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/english-variant_2.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/english-w_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/english-wo_accents.alias"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-variant_0.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-variant_0.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-variant_1.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-variant_1.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-variant_2.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-variant_2.rws"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-w_accents.multi"
  "${KDE4_LIB_INSTALL_DIR}/aspell-0.60/en-wo_accents.multi"
		DESTINATION ${BIN_INSTALL_DIR}/aspell-0.60 COMPONENT kdecore )
	
  install( FILES 
  "${QT_PLUGINS_DIR}/iconengines/qsvgicon4.dll"
		DESTINATION  ${BIN_INSTALL_DIR}/iconengines COMPONENT kdecore )
	

  install( FILES 
  "${QT_PLUGINS_DIR}/imageformats/qgif4.dll"
  "${QT_PLUGINS_DIR}/imageformats/qico4.dll"
  "${QT_PLUGINS_DIR}/imageformats/qjpeg4.dll"
  "${QT_PLUGINS_DIR}/imageformats/qmng4.dll"
  "${QT_PLUGINS_DIR}/imageformats/qsvg4.dll"
  "${QT_PLUGINS_DIR}/imageformats/qtiff4.dll"
		DESTINATION  ${BIN_INSTALL_DIR}/imageformats COMPONENT kdecore )


  install( FILES 
  "${QT_PLUGINS_DIR}/sqldrivers/qsqlite4.dll"
  "${QT_PLUGINS_DIR}/sqldrivers/qsqlmysql4.dll"
		DESTINATION  ${BIN_INSTALL_DIR}/sqldrivers COMPONENT kdecore )
  
#  install( FILES 
#   "${KDE4_INSTALL_DIR}/etc/dbus-1/session.conf"
#   "${KDE4_INSTALL_DIR}/etc/dbus-1/system.conf"
#		DESTINATION  ${SYSCONF_INSTALL_DIR}/dbus-1 COMPONENT kdecore )
  #install( FILES 
  # "${KDE4_INSTALL_DIR}/etc/dbus-1/system.d/org.kde.auth.conf"
 #  "${KDE4_INSTALL_DIR}/etc/dbus-1/system.d/org.kde.kcontrol.kcmremotewidgets.conf"
#		DESTINATION  ${SYSCONF_INSTALL_DIR}/dbus-1/system.d COMPONENT kdecore )
  
  install( FILES 
   "${KDE4_INSTALL_DIR}/etc/dbus-1/session.conf"
   "${KDE4_INSTALL_DIR}/etc/dbus-1/system.conf"
		DESTINATION  ${SYSCONF_INSTALL_DIR}/ COMPONENT kdecore )
  install( FILES 
   "${KDE4_INSTALL_DIR}/etc/dbus-1/system.d/org.kde.auth.conf"
   "${KDE4_INSTALL_DIR}/etc/dbus-1/system.d/org.kde.kcontrol.kcmremotewidgets.conf"
		DESTINATION  ${SYSCONF_INSTALL_DIR}/system.d COMPONENT kdecore )




  install( FILES 
  "${KDE4_INSTALL_DIR}/share/config/accept-languages.codes"
  "${KDE4_INSTALL_DIR}/share/config/emoticons.knsrc"
  "${KDE4_INSTALL_DIR}/share/config/icons.knsrc"
  "${KDE4_INSTALL_DIR}/share/config/kdebug.areas"
  "${KDE4_INSTALL_DIR}/share/config/kdebugrc"
  "${KDE4_INSTALL_DIR}/share/config/khotnewstuff.knsrc"
  #"${KDE4_INSTALL_DIR}/share/config/simonscenarios.knsrc"
  "${KDE4_INSTALL_DIR}/share/config/khotnewstuff_upload.knsrc"
		DESTINATION  ${DATA_INSTALL_DIR}/../config COMPONENT kdecore )


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/config/colors/40.colors"
  "${KDE4_INSTALL_DIR}/share/config/colors/Oxygen.colors"
  "${KDE4_INSTALL_DIR}/share/config/colors/Rainbow.colors"
  "${KDE4_INSTALL_DIR}/share/config/colors/Royal.colors"
  "${KDE4_INSTALL_DIR}/share/config/colors/Web.colors"
		DESTINATION  ${DATA_INSTALL_DIR}/../config/colors COMPONENT kdecore )
		
		
		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/fdl-notice.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/gpl-notice.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/help-menu.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/install-compile.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/install-intro.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/lgpl-notice.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/report-bugs.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/underArtisticLicense.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/underBSDLicense.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/underFDL.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/underGPL.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/underLGPL.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/underX11License.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/entities/update-doc.docbook"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/customization/de/entities COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/lang.entities"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/catalog.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/strings.entities"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/de/user.entities"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/customization/de COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/dtd/kdex.dtd"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/dtd/modifications.elements"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/dtd/rdbhier2.elements"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/dtd/rdbpool.elements"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/customization/dtd COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/fdl-notice.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/gpl-notice.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/help-menu.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/install-compile.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/install-intro.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/lgpl-notice.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/report-bugs.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/underArtisticLicense.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/underBSDLicense.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/underFDL.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/underGPL.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/underLGPL.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/underX11License.docbook"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/entities/update-doc.docbook"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/customization/en/entities COMPONENT kdecore )
  
		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/lang.entities"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/strings.entities"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/catalog.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/en/user.entities"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/customization/en COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/ca.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/cs.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/da.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/de.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/el.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/en.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/es.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/et.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/fi.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/fr.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/hu.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/it.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/ja.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/ko.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/nds.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/nl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/nn.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/no.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/pl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/pt.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/pt_br.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/README"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/ro.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/ru.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sk.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sr.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sr@ijekavian.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sr@ijekavianlatin.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sr@latin.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/sv.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/customization/xsl/zh_tw.xml"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/customization/xsl COMPONENT kdecore )
  
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/af.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ar.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/autoidx-ng.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/bg.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/bn.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/bs.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ca.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ChangeLog"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/common.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/cs.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/da.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/de.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/el.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/en.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/eo.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/es.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/et.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/eu.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/fi.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/fr.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/gentext.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/gl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/he.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/hr.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/hu.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/id.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/it.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ja.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ko.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/l10n.dtd"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/l10n.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/l10n.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/la.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/labels.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/lt.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/nb.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/nds.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/nl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/nn.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/olink.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/pi.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/pl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/pt.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/pt_br.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ro.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/ru.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sk.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sl.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sr.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sr_ijekavian.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sr_ijekavianlatin.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sr_latin.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/subtitles.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/sv.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/table.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/targetdatabase.dtd"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/targets.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/th.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/titles.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/tr.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/uk.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/vi.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/xh.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/zh_cn.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/common/zh_tw.xml"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/common COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/admon.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/autoidx-ng.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/autoidx.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/autotoc.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/biblio.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/block.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/callout.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/changebars.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/ChangeLog"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/chunk-code.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/chunk-common.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/chunk.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/chunker.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/chunkfast.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/chunktoc.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/component.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/division.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/docbook.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/docbookng.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/ebnf.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/footnote.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/formal.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/glossary.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/graphics.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/html-rtf.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/html.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/htmltbl.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/index.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/info.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/inline.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/keywords.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/lists.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/maketoc.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/manifest.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/math.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/oldchunker.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/onechunk.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/param.ent"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/param.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/param.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/param.xweb"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/pi.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/profile-chunk-code.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/profile-chunk.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/profile-docbook.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/profile-onechunk.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/qandaset.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/refentry.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/sections.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/synop.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/table.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/task.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/titlepage.templates.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/titlepage.templates.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/titlepage.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/toc.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/verbatim.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/html/xref.xsl"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/html COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/1.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/1.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/10.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/10.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/11.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/11.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/12.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/12.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/13.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/13.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/14.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/14.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/15.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/15.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/2.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/2.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/3.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/3.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/4.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/4.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/5.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/5.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/6.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/6.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/7.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/7.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/8.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/8.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/9.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/9.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/callouts/ChangeLog"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/images/callouts COMPONENT kdecore )
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/caution.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/caution.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/caution.tif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/blank.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/ChangeLog"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/draft.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/home.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/home.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/important.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/important.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/important.tif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/next.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/next.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/note.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/note.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/note.tif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/prev.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/prev.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/tip.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/tip.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/tip.tif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/toc-blank.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/toc-minus.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/toc-plus.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/up.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/up.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/warning.gif"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/warning.png"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/images/warning.tif"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/images COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/lib/ChangeLog"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/lib/lib.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/lib/lib.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/lib/lib.xweb"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/lib COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/manpages/docbook.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/manpages/lists.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/manpages/sect23.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/manpages/synop.xsl"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/manpages/xref.xsl"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/manpages COMPONENT kdecore )
  
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admon.graphics.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admon.graphics.path.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admon.graphics.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admon.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admon.textlabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admonition.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/admonition.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/alignment.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/annotate.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/appendix.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/arbortext.extensions.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/article.appendix.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/author.othername.in.middle.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/autotoc.label.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/axf.extensions.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/base.dir.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/biblioentry.item.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/bibliography.collection.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/bibliography.numbered.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/blockquote.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/body.font.family.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/body.font.master.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/body.font.size.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/body.margin.bottom.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/body.margin.top.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/bridgehead.in.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.defaultcolumn.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.graphics.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.graphics.number.limit.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.graphics.path.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.graphics.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.list.table.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.unicode.font.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.unicode.number.limit.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.unicode.start.character.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callout.unicode.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/callouts.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ChangeLog"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chapter.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.first.sections.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.quietly.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.section.depth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.sections.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.separate.lots.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunk.tocs.and.lots.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.cdata-section-elements.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.doctype-public.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.doctype-system.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.encoding.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.indent.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.media-type.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.method.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.omit-xml-declaration.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/chunker.output.standalone.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/citerefentry.link.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/collect.xref.targets.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.count.back.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.count.body.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.count.front.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.count.index.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.count.lot.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.count.titlepage.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.gap.back.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.gap.body.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.gap.front.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.gap.index.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.gap.lot.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/column.gap.titlepage.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/compact.list.item.spacing.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/component.label.includes.part.label.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/component.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/css.decoration.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/current.docid.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/default.float.class.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/default.image.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/default.table.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/default.units.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/dingbat.font.family.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/double.sided.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/draft.mode.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/draft.watermark.image.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ebnf.assignment.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ebnf.statement.terminator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ebnf.table.bgcolor.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ebnf.table.border.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/eclipse.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/eclipse.plugin.id.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/eclipse.plugin.name.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/eclipse.plugin.provider.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/emphasis.propagates.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/entry.propagates.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/equation.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/example.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/figure.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/firstterm.only.link.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footer.column.widths.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footer.content.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footer.rule.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footers.on.blank.pages.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footnote.font.size.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footnote.number.format.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footnote.number.symbols.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/footnote.sep.leader.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/fop.extensions.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/formal.object.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/formal.procedures.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/formal.title.placement.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/formal.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/funcsynopsis.decoration.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/funcsynopsis.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/funcsynopsis.tabular.threshold.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/function.parens.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.id.attributes.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.index.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.legalnotice.link.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.manifest.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.meta.abstract.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.section.toc.level.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/generate.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glossary.as.blocks.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glossary.collection.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glossentry.show.acronym.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glosslist.as.blocks.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glossterm.auto.link.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glossterm.separation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/glossterm.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/graphic.default.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/graphicsize.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/header.column.widths.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/header.content.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/header.rule.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/headers.on.blank.pages.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.base.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.cellpadding.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.cellspacing.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.cleanup.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.ext.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.extra.head.links.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.longdesc.link.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.longdesc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.stylesheet.type.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/html.stylesheet.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.alias.file.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.back.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.forward.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.hideshow.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.home.url.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.home.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.jump1.title.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.jump1.url.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.jump1.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.jump2.title.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.jump2.url.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.jump2.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.locate.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.next.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.options.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.prev.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.print.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.refresh.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.stop.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.button.zoom.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.chm.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.default.topic.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.display.progress.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.encoding.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.enhanced.decompilation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.enumerate.images.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.force.map.and.alias.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhc.binary.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhc.folders.instead.books.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhc.section.depth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhc.show.root.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhc.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhk.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhp.tail.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhp.window.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhp.windows.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.hhp.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.map.file.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.only.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.remember.window.position.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.show.advanced.search.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.show.favorities.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.show.menu.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.show.toolbar.text.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.title.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.use.hhk.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/htmlhelp.window.geometry.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/hyphenate.verbatim.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/hyphenate.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ignore.image.scaling.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/img.src.path.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/index.on.role.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/index.on.type.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/index.prefer.titleabbrev.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/index.preferred.page.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/informal.object.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/informalequation.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/informalexample.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/informalfigure.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/informaltable.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/inherit.keywords.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/insert.olink.page.number.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/insert.olink.pdf.frag.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/insert.xref.page.number.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/l10n.gentext.default.language.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/l10n.gentext.language.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/l10n.gentext.use.xref.language.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/label.from.part.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/line-height.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/linenumbering.everyNth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/linenumbering.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/linenumbering.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/linenumbering.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/link.mailto.url.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/list.block.spacing.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/list.item.spacing.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/make.graphic.viewport.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/make.index.markup.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/make.single.year.ranges.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/make.valid.html.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/make.year.ranges.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/manifest.in.base.dir.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/manifest.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/manual.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/marker.section.level.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/menuchoice.menu.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/menuchoice.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/monospace.font.family.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/monospace.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/monospace.verbatim.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/navig.graphics.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/navig.graphics.path.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/navig.graphics.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/navig.showtitles.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/nominal.image.depth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/nominal.image.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/nominal.table.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/normal.para.spacing.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.base.uri.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.debug.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.doctitle.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.fragid.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.lang.fallback.sequence.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.outline.ext.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.pubid.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.resolver.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/olink.sysid.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.height.portrait.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.height.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.margin.bottom.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.margin.inner.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.margin.outer.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.margin.top.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.orientation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.width.portrait.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/page.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/paper.type.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/para.propagates.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/part.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/passivetex.extensions.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/phrase.propagates.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/pixels.per.inch.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/points.per.em.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/preface.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/prefer.internal.olink.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/preferred.mediaobject.role.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/procedure.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/process.empty.source.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/process.source.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.arch.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.attribute.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.condition.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.conformance.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.lang.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.os.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.revision.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.revisionflag.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.role.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.security.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.userlevel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.value.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/profile.vendor.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/punct.honorific.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.defaultlabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.inherit.numeration.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.level1.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.level2.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.level3.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.level4.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.level5.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.level6.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qanda.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/qandadiv.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/refentry.generate.name.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/refentry.generate.title.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/refentry.pagebreak.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/refentry.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/refentry.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/refentry.xref.manvolnum.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/region.after.extent.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/region.before.extent.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/root.filename.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/root.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/rootid.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/runinhead.default.title.end.punct.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/runinhead.title.end.punct.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/sans.font.family.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/saxon.callouts.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/saxon.character.representation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/saxon.linenumbering.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/saxon.tablecolumns.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.autolabel.max.depth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.autolabel.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.label.includes.component.label.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.level1.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.level2.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.level3.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.level4.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.level5.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.level6.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.level1.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.level2.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.level3.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.level4.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.level5.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.level6.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/section.title.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/segmentedlist.as.table.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/shade.verbatim.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/shade.verbatim.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/show.comments.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/show.revisionflag.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/sidebar.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/simplesect.in.toc.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/spacing.paras.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/subscript.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/superscript.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/suppress.footer.navigation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/suppress.header.navigation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/suppress.navigation.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/symbol.font.family.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.borders.with.css.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.cell.border.color.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.cell.border.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.cell.border.thickness.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.cell.padding.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.entry.padding.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.footnote.number.format.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.footnote.number.symbols.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.frame.border.color.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.frame.border.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.frame.border.thickness.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/table.table.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/tablecolumns.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/target.database.document.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/targets.filename.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/template.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/tex.math.delims.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/tex.math.file.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/tex.math.in.alt.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/textdata.default.encoding.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/textinsert.extension.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/title.font.family.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/title.margin.left.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/toc.indent.width.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/toc.list.type.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/toc.margin.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/toc.max.depth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/toc.section.depth.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ulink.footnote.number.format.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ulink.footnotes.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ulink.hyphenate.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ulink.show.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/ulink.target.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.embed.for.svg.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.extensions.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.id.as.filename.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.id.function.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.local.olink.style.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.role.as.xrefstyle.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.role.for.mediaobject.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/use.svg.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/variablelist.as.blocks.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/variablelist.as.table.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/variablelist.max.termlength.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/verbatim.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/wordml.template.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xep.extensions.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xep.index.item.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xref.label-page.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xref.label-title.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xref.properties.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xref.title-page.separator.xml"
  "${KDE4_INSTALL_DIR}/share/apps/ksgmltools2/docbook/xsl/params/xref.with.number.and.title.xml"
      DESTINATION ${DATA_INSTALL_DIR}/ksgmltools2/docbook/xsl/params COMPONENT kdecore )

  
		

		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/khtml/khtml.rc"
  "${KDE4_INSTALL_DIR}/share/apps/khtml/domain_info"
  "${KDE4_INSTALL_DIR}/share/apps/khtml/error.html"
  "${KDE4_INSTALL_DIR}/share/apps/khtml/khtml_browser.rc"
		DESTINATION  ${DATA_INSTALL_DIR}/../apps/khtml COMPONENT kdecore )
		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/khtml/css/html4.css"
  "${KDE4_INSTALL_DIR}/share/apps/khtml/css/presentational.css"
  "${KDE4_INSTALL_DIR}/share/apps/khtml/css/quirks.css"
		DESTINATION  ${DATA_INSTALL_DIR}/../apps/khtml/css COMPONENT kdecore )
		

		
		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/khelpcenter/glossary.html.in"
  "${KDE4_INSTALL_DIR}/share/apps/khelpcenter/glossary.xslt"
  "${KDE4_INSTALL_DIR}/share/apps/khelpcenter/index.html.in"
  "${KDE4_INSTALL_DIR}/share/apps/khelpcenter/khelpcenterui.rc"
  "${KDE4_INSTALL_DIR}/share/apps/khelpcenter/table-of-contents.xslt"
		DESTINATION  ${DATA_INSTALL_DIR}/../apps/khelpcenter COMPONENT kdecore )
		
		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-bottom-left.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-bottom-middle.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-bottom-right.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-middle-left.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-middle-right.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-top-left.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-top-middle.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/bar-top-right.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/body-background.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-bottom-left.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-bottom-middle.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-bottom-right.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-center.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-middle-left.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-middle-right.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-top-left.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-top-middle.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/box-top-right.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/top-middle.png"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/kde_infopage.css"
  "${KDE4_INSTALL_DIR}/share/apps/kdeui/about/kde_infopage_rtl.css"
		DESTINATION  ${DATA_INSTALL_DIR}/../apps/kdeui/about COMPONENT kdecore )
	
		
		
  install( FILES 
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/1.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/2.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/3.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/4.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/5.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/6.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/7.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/8.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/9.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/10.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/artistic-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/block_title_bottom.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/block_title_mid.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/block_title_top.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bottom-left.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bottom-middle.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bottom-right.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bsd-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/doxygen.css"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/favicon.ico"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/fdl-license"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/fdl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/fdl-notice.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/footer.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/gpl-license"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/gpl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/header.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/kde_logo_bg.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/kde-default.css"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/lgpl-license"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/lgpl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/mainfooter.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/mainheader.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/qpl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/tabs.css"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-kde.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-left.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-left.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-middle.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-right.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-right.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/x11-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/xml.dcl"
		DESTINATION  ${DATA_INSTALL_DIR}/../doc/HTML/en/common COMPONENT kdecore )


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/1.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/2.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/3.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/4.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/5.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/6.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/7.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/8.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/9.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/10.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/artistic-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/block_title_bottom.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/block_title_mid.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/block_title_top.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bottom-left.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bottom-middle.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bottom-right.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/bsd-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/doxygen.css"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/favicon.ico"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/fdl-license"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/fdl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/fdl-notice.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/footer.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/gpl-license"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/gpl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/header.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/kde_logo_bg.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/kde-default.css"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/lgpl-license"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/lgpl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/mainfooter.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/mainheader.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/qpl-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/tabs.css"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-kde.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-left.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-left.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-middle.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-right.jpg"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/top-right.png"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/x11-license.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/en/common/xml.dcl"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/de/common/fdl-translated.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/de/common/gpl-translated.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/de/common/lgpl-translated.html"
  "${KDE4_INSTALL_DIR}/share/doc/HTML/de/common/kde-localised.css"
		DESTINATION  ${DATA_INSTALL_DIR}/../doc/HTML/de/common COMPONENT kdecore )

  install( FILES 
  "${KDE4_INSTALL_DIR}/share/config/ui/ui_standards.rc"
		DESTINATION  ${DATA_INSTALL_DIR}/../config/ui COMPONENT kdecore )





  install( FILES 
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.FavIcon.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.JobView.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.JobViewV2.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.JobViewServer.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KCookieServer.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.kded.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KdedGlobalAccel.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.kdialog.ProgressDialog.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KDirNotify.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.khelpcenter.kcmhelpcenter.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KHTMLPart.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KIMIface.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.kio.FileUndoManager.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KLauncher.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KMediaPlayer.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.Konqueror.Main.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.Konqueror.MainWindow.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.konqueror.Preloader.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KResourcesManager.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KSpeech.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KTimeZoned.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.KWallet.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.Phonon.AudioOutput.xml"
  #"${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.pim.IdentityManager.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.kde.Solid.Networking.Client.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.soprano.Model.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.soprano.NodeIterator.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.soprano.QueryResultIterator.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.soprano.Server.xml"
  "${KDE4_INSTALL_DIR}/share/dbus-1/interfaces/org.soprano.StatementIterator.xml"
		DESTINATION  ${DATA_INSTALL_DIR}/../dbus-1/interfaces COMPONENT kdecore )



  install( FILES 
  "${KDE4_INSTALL_DIR}/share/dbus-1/services/org.kde.knotify.service"
		DESTINATION  ${DATA_INSTALL_DIR}/../dbus-1/services COMPONENT kdecore )






  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/index.theme"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor COMPONENT kdecore )



  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/128x128/apps/knetattach.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/128x128/apps COMPONENT kdecore )






  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/16x16/actions/presence_away.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/16x16/actions/presence_offline.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/16x16/actions/presence_online.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/16x16/actions/presence_unknown.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/16x16/actions COMPONENT kdecore )



  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/16x16/apps/knetattach.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/16x16/apps COMPONENT kdecore )






  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/22x22/apps/knetattach.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/22x22/apps COMPONENT kdecore )






  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/32x32/actions/presence_away.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/32x32/actions/presence_offline.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/32x32/actions/presence_online.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/32x32/actions/presence_unknown.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/32x32/actions COMPONENT kdecore )



  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/32x32/apps/knetattach.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/32x32/apps COMPONENT kdecore )






  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/48x48/actions/presence_away.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/48x48/actions/presence_offline.png"
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/48x48/actions/presence_online.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/48x48/actions COMPONENT kdecore )



  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/48x48/apps/knetattach.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/48x48/apps COMPONENT kdecore )







  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/64x64/apps/knetattach.png"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/64x64/apps COMPONENT kdecore )








  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/hicolor/scalable/apps/knetattach.svgz"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/hicolor/scalable/apps COMPONENT kdecore )











  install( FILES 
  "${KDE4_INSTALL_DIR}/share/icons/oxygen/index.theme"
		DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen COMPONENT kdecore )



FILE(GLOB oxygen_actions_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/actions/*.png")
install (FILES ${oxygen_actions_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/apps/*.png")
install (FILES ${oxygen_apps_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/categories/*.png")
install (FILES ${oxygen_categories_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/devices/*.png")
install (FILES ${oxygen_devices_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/emblems/*.png")
install (FILES ${oxygen_emblems_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/emotes/*.png")
install (FILES ${oxygen_emotes_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/mimetypes/*.png")
install (FILES ${oxygen_mimetypes_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/places/*.png")
install (FILES ${oxygen_places_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_128 "${KDE4_INSTALL_DIR}/share/icons/oxygen/128x128/status/*.png")
install (FILES ${oxygen_status_128} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/128x128/status COMPONENT kdecore)
   



FILE(GLOB oxygen_actions_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/actions/*.png")
install (FILES ${oxygen_actions_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/apps/*.png")
install (FILES ${oxygen_apps_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/categories/*.png")
install (FILES ${oxygen_categories_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/devices/*.png")
install (FILES ${oxygen_devices_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/emblems/*.png")
install (FILES ${oxygen_emblems_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/emotes/*.png")
install (FILES ${oxygen_emotes_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/mimetypes/*.png")
install (FILES ${oxygen_mimetypes_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/places/*.png")
install (FILES ${oxygen_places_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_16 "${KDE4_INSTALL_DIR}/share/icons/oxygen/16x16/status/*.png")
install (FILES ${oxygen_status_16} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/16x16/status COMPONENT kdecore)
   
   


FILE(GLOB oxygen_actions_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/actions/*.png")
install (FILES ${oxygen_actions_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/apps/*.png")
install (FILES ${oxygen_apps_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/categories/*.png")
install (FILES ${oxygen_categories_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/devices/*.png")
install (FILES ${oxygen_devices_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/emblems/*.png")
install (FILES ${oxygen_emblems_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/emotes/*.png")
install (FILES ${oxygen_emotes_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/mimetypes/*.png")
install (FILES ${oxygen_mimetypes_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/places/*.png")
install (FILES ${oxygen_places_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_22 "${KDE4_INSTALL_DIR}/share/icons/oxygen/22x22/status/*.png")
install (FILES ${oxygen_status_22} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/22x22/status COMPONENT kdecore)
   


FILE(GLOB oxygen_actions_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/actions/*.png")
install (FILES ${oxygen_actions_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/apps/*.png")
install (FILES ${oxygen_apps_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/categories/*.png")
install (FILES ${oxygen_categories_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/devices/*.png")
install (FILES ${oxygen_devices_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/emblems/*.png")
install (FILES ${oxygen_emblems_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/emotes/*.png")
install (FILES ${oxygen_emotes_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/mimetypes/*.png")
install (FILES ${oxygen_mimetypes_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/places/*.png")
install (FILES ${oxygen_places_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_32 "${KDE4_INSTALL_DIR}/share/icons/oxygen/32x32/status/*.png")
install (FILES ${oxygen_status_32} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/32x32/status COMPONENT kdecore)
   
   
   


FILE(GLOB oxygen_actions_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/actions/*.png")
install (FILES ${oxygen_actions_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/apps/*.png")
install (FILES ${oxygen_apps_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/categories/*.png")
install (FILES ${oxygen_categories_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/devices/*.png")
install (FILES ${oxygen_devices_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/emblems/*.png")
install (FILES ${oxygen_emblems_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/emotes/*.png")
install (FILES ${oxygen_emotes_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/mimetypes/*.png")
install (FILES ${oxygen_mimetypes_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/places/*.png")
install (FILES ${oxygen_places_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_48 "${KDE4_INSTALL_DIR}/share/icons/oxygen/48x48/status/*.png")
install (FILES ${oxygen_status_48} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/48x48/status COMPONENT kdecore)
   
   
   


FILE(GLOB oxygen_actions_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/actions/*.png")
install (FILES ${oxygen_actions_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/apps/*.png")
install (FILES ${oxygen_apps_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/categories/*.png")
install (FILES ${oxygen_categories_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/devices/*.png")
install (FILES ${oxygen_devices_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/emblems/*.png")
install (FILES ${oxygen_emblems_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/emotes/*.png")
install (FILES ${oxygen_emotes_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/mimetypes/*.png")
install (FILES ${oxygen_mimetypes_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/places/*.png")
install (FILES ${oxygen_places_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_64 "${KDE4_INSTALL_DIR}/share/icons/oxygen/64x64/status/*.png")
install (FILES ${oxygen_status_64} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/64x64/status COMPONENT kdecore)
   
   
   


FILE(GLOB oxygen_actions_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/actions/*.svgz")
install (FILES ${oxygen_actions_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/actions COMPONENT kdecore)
   
FILE(GLOB oxygen_apps_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/apps/*.svgz")
install (FILES ${oxygen_apps_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/apps COMPONENT kdecore)
   
FILE(GLOB oxygen_categories_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/categories/*.svgz")
install (FILES ${oxygen_categories_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/categories COMPONENT kdecore)
   
FILE(GLOB oxygen_devices_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/devices/*.svgz")
install (FILES ${oxygen_devices_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/devices COMPONENT kdecore)
   
FILE(GLOB oxygen_emblems_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/emblems/*.svgz")
install (FILES ${oxygen_emblems_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/emblems COMPONENT kdecore)
   
FILE(GLOB oxygen_emotes_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/emotes/*.svgz")
install (FILES ${oxygen_emotes_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/emotes COMPONENT kdecore)
   
FILE(GLOB oxygen_mimetypes_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/mimetypes/*.svgz")
install (FILES ${oxygen_mimetypes_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/mimetypes COMPONENT kdecore)
   
FILE(GLOB oxygen_places_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/places/*.svgz")
install (FILES ${oxygen_places_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/places COMPONENT kdecore)
   
FILE(GLOB oxygen_status_scalable "${KDE4_INSTALL_DIR}/share/icons/oxygen/scalable/status/*.svgz")
install (FILES ${oxygen_status_scalable} 
   DESTINATION  ${DATA_INSTALL_DIR}/../icons/oxygen/scalable/status COMPONENT kdecore)



   
FILE(GLOB kde4_services "${KDE4_INSTALL_DIR}/share/kde4/services/*.protocol")
install (FILES ${kde4_services} DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services COMPONENT kdecore)
FILE(GLOB kde4_services "${KDE4_INSTALL_DIR}/share/kde4/services/*.desktop")
install (FILES ${kde4_services} DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services COMPONENT kdecore)
   
FILE(GLOB kde4_services_kded "${KDE4_INSTALL_DIR}/share/kde4/services/kded/*.desktop")
install (FILES ${kde4_services_kded} DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services/kded COMPONENT kdecore)


  #install( FILES 
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kabc_manager.desktop"
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kcal_manager.desktop"
	#	DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services/kresources COMPONENT kdecore )


  #install( FILES 
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kabc/dir.desktop"
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kabc/file.desktop"
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kabc/ldapkio.desktop"
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kabc/net.desktop"
#		DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services/kresources/kabc COMPONENT kdecore )



  #install( FILES 
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kcal/local.desktop"
  #"${KDE4_INSTALL_DIR}/share/kde4/services/kresources/kcal/localdir.desktop"
	#	DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services/kresources/kcal COMPONENT kdecore )




  #install( FILES 
  #"${KDE4_INSTALL_DIR}/share/kde4/services/phononbackends/ds9.desktop"
#		DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services/phononbackends COMPONENT kdecore )


   
FILE(GLOB kde4_services_qimageioplugins "${KDE4_INSTALL_DIR}/share/kde4/services/qimageioplugins/*.desktop")
install (FILES ${kde4_services_qimageioplugins} DESTINATION  ${DATA_INSTALL_DIR}/../kde4/services/qimageioplugins COMPONENT kdecore)
   
FILE(GLOB kde4_servicetypes "${KDE4_INSTALL_DIR}/share/kde4/servicetypes/*.desktop")
install (FILES ${kde4_servicetypes} DESTINATION  ${DATA_INSTALL_DIR}/../kde4/servicetypes COMPONENT kdecore)



  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/all_languages"
  #"${KDE4_INSTALL_DIR}/share/locale/locale.alias"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale COMPONENT kdecore )


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/de/entry.desktop"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale/de COMPONENT kdecore )


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/attica_kde.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/display.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/drkonqi.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/filetypes.mo"
  #"${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/gettext-runtime.mo"
  #"${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/gettext-tools.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kcminfo.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kcminit.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kcmstyle.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kcm_attica.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kcolorchooser.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kdebugdialog.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kdelibs_colors4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kdelibs4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kde-menu.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kdeqt.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kdialog.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kfile.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kfmclient.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/khelpcenter.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_archive.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_audiocd.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_finger.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_fish.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_floppy.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_groupwise.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_help4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_imap4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_jabberdisco.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_ldap.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_man.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_mbox.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_nfs.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_nntp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_pop3.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_remote.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_sftp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_sieve.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_smb.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_smtp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_svn.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_thumbnail.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_trash.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio_zeroconf.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kio4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kioclient.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kioexec.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/knotify4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kquitapp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kstyle_config.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kstyle_phase_config.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kuiserver.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kurifilter.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/kuser.mo"
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_MESSAGES/lokalize.mo"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale/de/LC_MESSAGES COMPONENT kdecore )





  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/de/LC_SCRIPTS/kdelibs4/kdelibs4.js"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale/de/LC_SCRIPTS/kdelibs4 COMPONENT kdecore )




		


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/fr/entry.desktop"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale/fr COMPONENT kdecore )


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/attica_kde.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/display.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/drkonqi.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/filetypes.mo"
  #"${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/gettext-runtime.mo"
  #"${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/gettext-tools.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kcminfo.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kcminit.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kcmstyle.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kcm_attica.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kcolorchooser.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kdebugdialog.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kdelibs_colors4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kdelibs4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kde-menu.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kdeqt.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kdialog.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kfile.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kfmclient.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/khelpcenter.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_archive.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_audiocd.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_finger.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_fish.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_floppy.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_groupwise.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_help4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_imap4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_jabberdisco.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_ldap.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_man.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_mbox.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_nfs.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_nntp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_pop3.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_remote.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_sftp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_sieve.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_smb.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_smtp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_svn.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_thumbnail.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_trash.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio_zeroconf.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kio4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kioclient.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kioexec.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/knotify4.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kquitapp.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kstyle_config.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kstyle_phase_config.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kuiserver.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kurifilter.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/kuser.mo"
  "${KDE4_INSTALL_DIR}/share/locale/fr/LC_MESSAGES/lokalize.mo"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale/fr/LC_MESSAGES COMPONENT kdecore )









  #install( FILES 
  #"${KDE4_INSTALL_DIR}/share/locale/en@boldquot/LC_MESSAGES/gettext-runtime.mo"
  #"${KDE4_INSTALL_DIR}/share/locale/en@boldquot/LC_MESSAGES/gettext-tools.mo"
	#	DESTINATION  ${DATA_INSTALL_DIR}/../locale/en@boldquot/LC_MESSAGES COMPONENT kdecore )






  #install( FILES 
  #"${KDE4_INSTALL_DIR}/share/locale/en@quot/LC_MESSAGES/gettext-runtime.mo"
  #"${KDE4_INSTALL_DIR}/share/locale/en@quot/LC_MESSAGES/gettext-tools.mo"
	#	DESTINATION  ${DATA_INSTALL_DIR}/../locale/en@quot/LC_MESSAGES COMPONENT kdecore )




  install( FILES 
  "${KDE4_INSTALL_DIR}/share/locale/en_US/entry.desktop"
		DESTINATION  ${DATA_INSTALL_DIR}/../locale/en_US COMPONENT kdecore )




  install( FILES 
  "${KDE4_INSTALL_DIR}/share/mime/aliases"
  "${KDE4_INSTALL_DIR}/share/mime/generic-icons"
  "${KDE4_INSTALL_DIR}/share/mime/globs"
  "${KDE4_INSTALL_DIR}/share/mime/globs2"
  "${KDE4_INSTALL_DIR}/share/mime/icons"
  "${KDE4_INSTALL_DIR}/share/mime/magic"
  "${KDE4_INSTALL_DIR}/share/mime/mime.cache"
  "${KDE4_INSTALL_DIR}/share/mime/subclasses"
  "${KDE4_INSTALL_DIR}/share/mime/treemagic"
  "${KDE4_INSTALL_DIR}/share/mime/types"
  "${KDE4_INSTALL_DIR}/share/mime/XMLnamespaces"
		DESTINATION  ${DATA_INSTALL_DIR}/../mime COMPONENT kdecore )


  install( FILES 
  "${KDE4_INSTALL_DIR}/share/mime/all/all.xml"
  "${KDE4_INSTALL_DIR}/share/mime/all/allfiles.xml"
		DESTINATION  ${DATA_INSTALL_DIR}/../mime/all COMPONENT kdecore )


FILE(GLOB mime_application "${KDE4_INSTALL_DIR}/share/mime/application/*.xml")
install (FILES ${mime_application} DESTINATION  ${DATA_INSTALL_DIR}/../mime/application COMPONENT kdecore)

FILE(GLOB mime_audio "${KDE4_INSTALL_DIR}/share/mime/audio/*.xml")
install (FILES ${mime_audio} DESTINATION  ${DATA_INSTALL_DIR}/../mime/audio COMPONENT kdecore)

FILE(GLOB mime_fonts "${KDE4_INSTALL_DIR}/share/mime/fonts/*.xml")
install (FILES ${mime_fonts} DESTINATION  ${DATA_INSTALL_DIR}/../mime/fonts COMPONENT kdecore)

FILE(GLOB mime_image "${KDE4_INSTALL_DIR}/share/mime/image/*.xml")
install (FILES ${mime_image} DESTINATION  ${DATA_INSTALL_DIR}/../mime/image COMPONENT kdecore)

FILE(GLOB mime_inode "${KDE4_INSTALL_DIR}/share/mime/inode/*.xml")
install (FILES ${mime_inode} DESTINATION  ${DATA_INSTALL_DIR}/../mime/inode COMPONENT kdecore)

FILE(GLOB mime_interface "${KDE4_INSTALL_DIR}/share/mime/interface/*.xml")
install (FILES ${mime_interface} DESTINATION  ${DATA_INSTALL_DIR}/../mime/interface COMPONENT kdecore)

FILE(GLOB mime_message "${KDE4_INSTALL_DIR}/share/mime/message/*.xml")
install (FILES ${mime_message} DESTINATION  ${DATA_INSTALL_DIR}/../mime/message COMPONENT kdecore)

FILE(GLOB mime_model "${KDE4_INSTALL_DIR}/share/mime/model/*.xml")
install (FILES ${mime_model} DESTINATION  ${DATA_INSTALL_DIR}/../mime/model COMPONENT kdecore)

FILE(GLOB mime_multipart "${KDE4_INSTALL_DIR}/share/mime/multipart/*.xml")
install (FILES ${mime_multipart} DESTINATION  ${DATA_INSTALL_DIR}/../mime/multipart COMPONENT kdecore)

FILE(GLOB mime_packages "${KDE4_INSTALL_DIR}/share/mime/packages/*.xml")
install (FILES ${mime_packages} DESTINATION  ${DATA_INSTALL_DIR}/../mime/packages COMPONENT kdecore)

FILE(GLOB mime_text "${KDE4_INSTALL_DIR}/share/mime/text/*.xml")
install (FILES ${mime_text} DESTINATION  ${DATA_INSTALL_DIR}/../mime/text COMPONENT kdecore)

FILE(GLOB mime_uri "${KDE4_INSTALL_DIR}/share/mime/uri/*.xml")
install (FILES ${mime_uri} DESTINATION  ${DATA_INSTALL_DIR}/../mime/uri COMPONENT kdecore)

FILE(GLOB mime_video "${KDE4_INSTALL_DIR}/share/mime/video/*.xml")
install (FILES ${mime_video} DESTINATION  ${DATA_INSTALL_DIR}/../mime/video COMPONENT kdecore)

FILE(GLOB mime_x-content "${KDE4_INSTALL_DIR}/share/mime/x-content/*.xml")
install (FILES ${mime_x-content} DESTINATION  ${DATA_INSTALL_DIR}/../mime/x-content COMPONENT kdecore)

FILE(GLOB mime_x-epoc "${KDE4_INSTALL_DIR}/share/mime/x-epoc/*.xml")
install (FILES ${mime_x-epoc} DESTINATION  ${DATA_INSTALL_DIR}/../mime/x-epoc COMPONENT kdecore)

ENDIF(WIN32)
