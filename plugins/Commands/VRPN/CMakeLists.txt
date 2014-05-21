include_directories(${VRPN_INCLUDE_DIR})

set (simonvrpncommandplugin_SRCS
  vrpncommandmanager.cpp
  vrpnconfiguration.cpp
  vrpncommand.cpp
  createvrpncommandwidget.cpp
)

kde4_add_ui_files(simonvrpncommandplugin_SRCS
  vrpnconfigurationdlg.ui
  createvrpncommandwidget.ui
)

kde4_add_plugin(simonvrpncommandplugin ${simonvrpncommandplugin_SRCS})

target_link_libraries(simonvrpncommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  simonscenarios simonactions ${VRPN_LIBRARY})

install(TARGETS simonvrpncommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandvrpnplugin)
install(FILES simonvrpncommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandvrpnplugin)
