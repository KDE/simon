set (simoncompositecommandplugin_SRCS 
  createcompositecommandwidget.cpp 
  compositecommand.cpp 
  delaycommand.cpp 
  compositecommandmanager.cpp 
)

ki18n_wrap_ui(simoncompositecommandplugin_SRCS
  createcompositecommandwidget.ui
)

kde4_add_plugin(simoncompositecommandplugin MODULE ${simoncompositecommandplugin_SRCS})

target_link_libraries(simoncompositecommandplugin
  KF5::KIOCore ${QT_LIBRARIES} simonscenarios simonactions simonlogging simonscenariobase)

install(TARGETS simoncompositecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandcompositeplugin)
install(FILES simoncompositecommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcompositeplugin)
