set (simonamarokcommandplugin_SRCS
  amarokcommandmanager.cpp
  amarokconfiguration.cpp
  amarokcommand.cpp
  createamarokcommandwidget.cpp
)

kde4_add_ui_files(simonamarokcommandplugin_SRCS 
  amarokconfigurationdlg.ui
  createamarokcommandwidget.ui
)

kde4_add_plugin(simonamarokcommandplugin ${simonamarokcommandplugin_SRCS})

target_link_libraries(simonamarokcommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  simonscenarios simonactions)

install(TARGETS simonamarokcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandamarokplugin)
install(FILES simonamarokcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandamarokplugin)
