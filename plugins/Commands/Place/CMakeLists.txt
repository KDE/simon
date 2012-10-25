set (simonplacecommandplugin_SRCS 
  createplacecommandwidget.cpp 
  placecommand.cpp 
  placecommandmanager.cpp 
  selectplacedialog.cpp
)

kde4_add_ui_files(simonplacecommandplugin_SRCS 
  selectplacedlg.ui
  createplacecommandwidget.ui
)

kde4_add_plugin(simonplacecommandplugin ${simonplacecommandplugin_SRCS})

target_link_libraries(simonplacecommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  simonscenarios simonscenariobase simonlogging)

install(TARGETS simonplacecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandplaceplugin)
install(FILES simonplacecommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandplaceplugin)
