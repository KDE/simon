set (simondialogcommandplugin_SRCS 
  dialogcommandmanager.cpp 
  dialogconfiguration.cpp
  dialogcommand.cpp
  createdialogcommandwidget.cpp
)

kde4_add_ui_files(simondialogcommandplugin_SRCS 
  dialogwidget.ui
  dialogconfigurationdlg.ui
  createdialogcommandwidget.ui
)

kde4_add_plugin(simondialogcommandplugin ${simondialogcommandplugin_SRCS})

target_link_libraries(simondialogcommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  simonscenarios simonactions)

install(TARGETS simondialogcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddialogplugin)
install(FILES simondialogcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddialogplugin)