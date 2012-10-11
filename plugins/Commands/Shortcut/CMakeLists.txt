set (simonshortcutcommandplugin_SRCS 
  createshortcutcommandwidget.cpp 
  shortcutcommand.cpp 
  shortcutcommandmanager.cpp 
)

kde4_add_ui_files(simonshortcutcommandplugin_SRCS createshortcutcommandwidget.ui)

kde4_add_plugin(simonshortcutcommandplugin ${simonshortcutcommandplugin_SRCS})

target_link_libraries(simonshortcutcommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  ${QT_LIBRARIES} simonscenariobase simonscenarios eventsimulation simonlogging)

install(TARGETS simonshortcutcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandshortcutplugin)
install(FILES simonshortcutcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandshortcutplugin)

