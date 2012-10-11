set (simoncompositecommandplugin_SRCS 
  createcompositecommandwidget.cpp 
  compositecommand.cpp 
  delaycommand.cpp 
  compositecommandmanager.cpp 
)

kde4_add_ui_files(simoncompositecommandplugin_SRCS 
  createcompositecommandwidget.ui
)

kde4_add_plugin(simoncompositecommandplugin ${simoncompositecommandplugin_SRCS})

target_link_libraries(simoncompositecommandplugin ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS} ${QT_LIBRARIES} simonscenarios simonactions simonlogging simonscenariobase)

install(TARGETS simoncompositecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandcompositeplugin)
install(FILES simoncompositecommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcompositeplugin)
