include_directories(${QACCESSIBILITYCLIENT_INCLUDE_DIR})

set(simonatspicommandplugin_SRCS
  atspicommandmanager.cpp
  atspiconfiguration.cpp
  atspiscanner.cpp
)

kde4_add_ui_files(simonatspicommandplugin_SRCS  atspiconfigurationdlg.ui)

kde4_add_plugin(simonatspicommandplugin ${simonatspicommandplugin_SRCS})

target_link_libraries(simonatspicommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  ${QACCESSIBILITYCLIENT_LIBRARY} simonscenarios simonactions)

install(TARGETS simonatspicommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandatspiplugin)
install(FILES simonatspicommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandatspiplugin)

add_subdirectory(test)