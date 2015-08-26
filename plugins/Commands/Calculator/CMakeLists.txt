set (simoncalculatorplugin_SRCS 
  calculatorcommandmanager.cpp
  token.cpp
  calculatorconfiguration.cpp
)

ki18n_wrap_ui(simoncalculatorplugin_SRCS calculatorconfigurationdlg.ui calculatorwidget.ui)

kde4_add_plugin(simoncalculatorplugin MODULE ${simoncalculatorplugin_SRCS})

target_link_libraries(simoncalculatorplugin  Qt5::Xml
  eventsimulation simonlogging simonscenarios simonactions
  simoninfo simonscenariobase)

install(TARGETS simoncalculatorplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin)
install(FILES simoncalculatorcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin)
