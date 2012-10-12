set (simonjsoncommandplugin_SRCS 
  jsoncommandmanager.cpp 
  jsonconfiguration.cpp
  jsoncommand.cpp
  createjsoncommandwidget.cpp
)

kde4_add_ui_files(simonjsoncommandplugin_SRCS 
  jsonconfigurationdlg.ui
  createjsoncommandwidget.ui
)

kde4_add_plugin(simonjsoncommandplugin ${simonjsoncommandplugin_SRCS})

target_link_libraries(simonjsoncommandplugin ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY}
  simonscenarios simonscenariobase simonjsonconnector)

install(TARGETS simonjsoncommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandjsonplugin)
install(FILES simonjsoncommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandjsonplugin)
