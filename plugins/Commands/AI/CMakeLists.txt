set (simonaiplugin_SRCS 
  aicommandmanager.cpp
  aiconfiguration.cpp
  aimlparser.cpp
)

ki18n_wrap_ui(simonaiplugin_SRCS aiconfigurationdlg.ui)

kde4_add_plugin(simonaiplugin MODULE ${simonaiplugin_SRCS})

target_link_libraries(simonaiplugin  Qt5::Xml
  ${QT_LIBRARIES} simonlogging simonscenarios simontts simonactions)

install(TARGETS simonaiplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandaiplugin)
install(FILES simonaiplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandaiplugin)
install(FILES bot.xml vars.xml substitutions.xml  DESTINATION ${DATA_INSTALL_DIR}/ai/util COMPONENT simoncommandaiplugin)

add_subdirectory(aimls)
