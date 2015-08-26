set (simonexecutablecommandplugin_SRCS 
  createexecutablecommandwidget.cpp 
  executablecommand.cpp 
  executablecommandmanager.cpp 
)

ki18n_wrap_ui(simonexecutablecommandplugin_SRCS createexecutablecommandwidget.ui)

kde4_add_plugin(simonexecutablecommandplugin ${simonexecutablecommandplugin_SRCS})

target_link_libraries(simonexecutablecommandplugin  KF5::KIOCore simonscenarios simonscenariobase simonlogging simonuicomponents)

install(TARGETS simonexecutablecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandexecutableplugin)
install(FILES simonexecutablecommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandexecutableplugin)
