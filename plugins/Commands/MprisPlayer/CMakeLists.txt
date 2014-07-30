set (simonmprisplayerplugin_SRCS
  mprisplayercommandmanager.cpp
  mprisplayerconfiguration.cpp
  mprisplayercommand.cpp
  createmprisplayercommandwidget.cpp
  player.cpp
)

kde4_add_ui_files(simonmprisplayerplugin_SRCS
  mprisplayerconfigurationdlg.ui
  createmprisplayercommandwidget.ui
)

kde4_add_plugin(simonmprisplayerplugin ${simonmprisplayerplugin_SRCS})

target_link_libraries(simonmprisplayerplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  simonscenarios simonactions)

install(TARGETS simonmprisplayerplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandmprisplayerplugin)
install(FILES simonmprisplayerplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandmprisplayerplugin)
