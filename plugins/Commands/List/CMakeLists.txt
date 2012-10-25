set (simonlistcommandplugin_SRCS 
  createlistcommandwidget.cpp 
  listcommandmanager.cpp 
)

kde4_add_ui_files(simonlistcommandplugin_SRCS 
  createlistcommandwidget.ui
)

kde4_add_plugin(simonlistcommandplugin ${simonlistcommandplugin_SRCS})

target_link_libraries(simonlistcommandplugin ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS} simonscenarios simonscenariobase
  simonlogging simonactions)

install(TARGETS simonlistcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandlistplugin)
install(FILES simonlistcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandlistplugin)
