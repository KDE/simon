set (simonfilterplugin_SRCS 
  filtercommandmanager.cpp
  filterconfiguration.cpp 
)

kde4_add_ui_files(simonfilterplugin_SRCS filterconfigurationdlg.ui)

kde4_add_plugin(simonfilterplugin ${simonfilterplugin_SRCS})

target_link_libraries(simonfilterplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  eventsimulation simonlogging simonscenarios simonactions simoninfo simonscenariobase)

install(TARGETS simonfilterplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandfilterplugin)
install(FILES simonfilterplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandfilterplugin)
