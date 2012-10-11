set (simonaiplugin_SRCS 
  aicommandmanager.cpp
  aiconfiguration.cpp
  aimlparser.cpp
)

kde4_add_ui_files(simonaiplugin_SRCS aiconfigurationdlg.ui)

kde4_add_plugin(simonaiplugin ${simonaiplugin_SRCS})

target_link_libraries(simonaiplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY} ${QT_LIBRARIES} simonlogging simonscenarios simonscenariobase)

install(TARGETS simonaiplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandaiplugin)
install(FILES simonaiplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandaiplugin)
install(FILES bot.xml vars.xml substitutions.xml  DESTINATION ${DATA_INSTALL_DIR}/ai/util COMPONENT simoncommandaiplugin)

add_subdirectory(aimls)