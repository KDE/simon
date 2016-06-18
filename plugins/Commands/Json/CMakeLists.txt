set (simonjsoncommandplugin_SRCS 
  jsoncommandmanager.cpp 
  jsonconfiguration.cpp
  jsoncommand.cpp
  createjsoncommandwidget.cpp
)

ki18n_wrap_ui(simonjsoncommandplugin_SRCS
  jsonconfigurationdlg.ui
  createjsoncommandwidget.ui
)

kde4_add_library(simonjsoncommandplugin MODULE ${simonjsoncommandplugin_SRCS})

target_link_libraries(simonjsoncommandplugin
  KF5::KIOCore Qt5::Xml
  simonscenarios simonjsonconnector)

install(TARGETS simonjsoncommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandjsonplugin)
install(FILES simonjsoncommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandjsonplugin)
