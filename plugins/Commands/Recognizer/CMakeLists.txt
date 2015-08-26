set (simonrecognizercommandplugin_SRCS 
  recognizercommandmanager.cpp 
  recognizerconfiguration.cpp
)


ki18n_wrap_ui(simonrecognizercommandplugin_SRCS recognizerconfigurationdlg.ui)

kde4_add_plugin(simonrecognizercommandplugin ${simonrecognizercommandplugin_SRCS})

target_link_libraries(simonrecognizercommandplugin
  Qt5::Xml simonscenarios eventsimulation simonscenariobase)

install(TARGETS simonrecognizercommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandrecognizerplugin)
install(FILES simonrecognizercommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandrecognizerplugin)
