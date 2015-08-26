set (simondialogcommandplugin_SRCS 
  dialogcommandmanager.cpp 
  dialogconfiguration.cpp
  dialogcommand.cpp
  createdialogcommandwidget.cpp
)

ki18n_wrap_ui(simondialogcommandplugin_SRCS
  dialogwidget.ui
  dialogconfigurationdlg.ui
  createdialogcommandwidget.ui
)

kde4_add_pluginf(simondialogcommandplugin ${simondialogcommandplugin_SRCS})

target_link_libraries(simondialogcommandplugin  Qt5::Xml
  simonscenarios simonactions)

install(TARGETS simondialogcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddialogplugin)
install(FILES simondialogcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddialogplugin)
