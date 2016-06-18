set (simonlistcommandplugin_SRCS 
  createlistcommandwidget.cpp 
  listcommandmanager.cpp 
)

ki18n_wrap_ui(simonlistcommandplugin_SRCS
  createlistcommandwidget.ui
)

kde4_add_plugin(simonlistcommandplugin ${simonlistcommandplugin_SRCS})

target_link_libraries(simonlistcommandplugin
  KF5::KIOCore simonscenarios
  simonlogging simonactions)

install(TARGETS simonlistcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandlistplugin)
install(FILES simonlistcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandlistplugin)
