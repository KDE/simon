set (simondictationcommandplugin_SRCS 
  dictationcommandmanager.cpp 
  dictationconfiguration.cpp
)

ki18n_wrap_ui(simondictationcommandplugin_SRCS dictationconfigurationdlg.ui)

kde4_add_library(simondictationcommandplugin MODULE ${simondictationcommandplugin_SRCS})

target_link_libraries(simondictationcommandplugin
  Qt5::Xml simonscenarios eventsimulation simonactions)

install(TARGETS simondictationcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddictationplugin)
install(FILES simondictationcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddictationplugin)
