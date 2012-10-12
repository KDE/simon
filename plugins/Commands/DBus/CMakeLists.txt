set (simondbuscommandplugin_SRCS 
  dbuscommandmanager.cpp 
  dbuscommand.cpp
  createdbuscommandwidget.cpp
)

kde4_add_ui_files(simondbuscommandplugin_SRCS 
  createdbuscommandwidget.ui
)

kde4_add_plugin(simondbuscommandplugin ${simondbuscommandplugin_SRCS})

target_link_libraries(simondbuscommandplugin ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY}
  simonscenarios simonscenariobase)

install(TARGETS simondbuscommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddbusplugin)
install(FILES simondbuscommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddbusplugin)
