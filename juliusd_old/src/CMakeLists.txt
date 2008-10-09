set (	juliusdSources
                databaseaccess.cpp
                juliuscontrol.cpp
                juliusd.cpp
                networkcontrol.cpp
                newuserdialog.cpp
                projectview.cpp
                settings.cpp
                settingsview.cpp
                simonsocket.cpp
                sync_factory.cpp
                sync_filereader.cpp
                sync_filewriter.cpp
                sync_merger.cpp
                sync_networkconfiguration.cpp
                sync_networkreader.cpp
                sync_networkwriter.cpp
                sync_reader.cpp
                sync_syncmanager.cpp
                sync_writer.cpp
                trayiconmanager.cpp
                userdialog.cpp
		)

kde4_add_ui_files( juliusdSources 
                main.ui
                newuser.ui
                settings.ui
                users.ui
		)

kde4_add_executable( juliusd ${juliusdSources} )

target_link_libraries( juliusd ${KDE4_KDEUI_LIBS} ${QT_LIBRARIES} )

install (TARGETS juliusd DESTINATION ${BIN_INSTALL_DIR})

