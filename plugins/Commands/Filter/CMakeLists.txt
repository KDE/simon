set (simonfilterplugin_SRCS 
  filtercommandmanager.cpp
  filterconfiguration.cpp 
)

ki18n_wrap_ui(simonfilterplugin_SRCS filterconfigurationdlg.ui)

kde4_add_plugin(simonfilterplugin ${simonfilterplugin_SRCS})

target_link_libraries(simonfilterplugin  Qt5::Xml
  eventsimulation simonlogging simonscenarios simonactions simoninfo simonscenariobase)

install(TARGETS simonfilterplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandfilterplugin)
install(FILES simonfilterplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandfilterplugin)
