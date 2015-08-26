include_directories(${VRPN_INCLUDE_DIRS})
add_definitions(${VRPN_CLIENT_DEFINITIONS})

set (simonvrpncommandplugin_SRCS
  vrpncommandmanager.cpp
  vrpnconfiguration.cpp
  vrpncommand.cpp
  createvrpncommandwidget.cpp
)

ki18n_wrap_ui(simonvrpncommandplugin_SRCS
  vrpnconfigurationdlg.ui
  createvrpncommandwidget.ui
)

kde4_add_plugin(simonvrpncommandplugin ${simonvrpncommandplugin_SRCS})

target_link_libraries(simonvrpncommandplugin  Qt5::Xml
  simonscenarios simonactions ${VRPN_LIBRARIES})

install(TARGETS simonvrpncommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandvrpnplugin)
install(FILES simonvrpncommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandvrpnplugin)
