set (simontextmacrocommandplugin_SRCS 
  createtextmacrocommandwidget.cpp 
  textmacrocommand.cpp 
  textmacrocommandmanager.cpp 
)

ki18n_wrap_ui(simontextmacrocommandplugin_SRCS createtextmacrocommandwidget.ui)

kde4_add_plugin(simontextmacrocommandplugin ${simontextmacrocommandplugin_SRCS})

target_link_libraries(simontextmacrocommandplugin  KF5::KIOCore
  simonscenarios eventsimulation simonlogging simonactions)

install(TARGETS simontextmacrocommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandtextmacroplugin)
install(FILES simontextmacrocommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandtextmacroplugin)
