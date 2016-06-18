set (simondbuscommandplugin_SRCS 
  dbuscommandmanager.cpp 
  dbuscommand.cpp
  createdbuscommandwidget.cpp
)

ki18n_wrap_ui(simondbuscommandplugin_SRCS 
  createdbuscommandwidget.ui
)

kde4_add_plugin(simondbuscommandplugin MODULE ${simondbuscommandplugin_SRCS})

target_link_libraries(simondbuscommandplugin 
  KF5::KIOCore Qt5::Xml
  simonscenarios)

install(TARGETS simondbuscommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddbusplugin)
install(FILES simondbuscommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddbusplugin)
