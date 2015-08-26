set (simonmprisplayerplugin_SRCS
  mprisplayercommandmanager.cpp
  mprisplayerconfiguration.cpp
  mprisplayercommand.cpp
  createmprisplayercommandwidget.cpp
  player.cpp
)

ki18n_wrap_ui(simonmprisplayerplugin_SRCS
  mprisplayerconfigurationdlg.ui
  createmprisplayercommandwidget.ui
)

kde4_add_library(simonmprisplayerplugin MODULE ${simonmprisplayerplugin_SRCS})

target_link_libraries(simonmprisplayerplugin  Qt5::Xml
  simonscenarios simonactions)

install(TARGETS simonmprisplayerplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandmprisplayerplugin)
install(FILES simonmprisplayerplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandmprisplayerplugin)
