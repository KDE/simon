set (simonexecutablecommandplugin_SRCS 
  createexecutablecommandwidget.cpp 
  executablecommand.cpp 
  executablecommandmanager.cpp 
)

kde4_add_ui_files(simonexecutablecommandplugin_SRCS createexecutablecommandwidget.ui)

kde4_add_plugin(simonexecutablecommandplugin ${simonexecutablecommandplugin_SRCS})

target_link_libraries(simonexecutablecommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonscenarios simonscenariobase simonlogging simonuicomponents)

install(TARGETS simonexecutablecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandexecutableplugin)
install(FILES simonexecutablecommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandexecutableplugin)
