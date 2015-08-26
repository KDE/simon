set (simonshortcutcommandplugin_SRCS 
  createshortcutcommandwidget.cpp 
  shortcutcommand.cpp 
  shortcutcommandmanager.cpp 
)

ki18n_wrap_ui(simonshortcutcommandplugin_SRCS createshortcutcommandwidget.ui)

kde4_add_plugin(simonshortcutcommandplugin ${simonshortcutcommandplugin_SRCS})

target_link_libraries(simonshortcutcommandplugin  KF5::KIOCore
  ${QT_LIBRARIES} simonscenariobase simonscenarios eventsimulation simonlogging)

install(TARGETS simonshortcutcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandshortcutplugin)
install(FILES simonshortcutcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandshortcutplugin)
