set (simondictationcommandplugin_SRCS 
  dictationcommandmanager.cpp 
  dictationconfiguration.cpp
  replacements.cpp
  replacement.cpp
  dictationreplacementui.cpp
)

kde4_add_ui_files(simondictationcommandplugin_SRCS dictationconfigurationdlg.ui dictationreplacementui.ui)

kde4_add_plugin(simondictationcommandplugin ${simondictationcommandplugin_SRCS})

if(QAccessibilityClient_FOUND)
add_definitions(-DATSPI_ENABLED)
set (simondialogcommandplugin_ExtraLibs ${QACCESSIBILITYCLIENT_LIBRARY})
endif()
target_link_libraries(simondictationcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simonscenarios eventsimulation simonscenariobase ${simondialogcommandplugin_ExtraLibs})

install(TARGETS simondictationcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddictationplugin)
install(FILES simondictationcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddictationplugin)
