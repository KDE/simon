include_directories(${QACCESSIBILITYCLIENT_INCLUDE_DIR})

set(simonatspicommandplugin_SRCS
  atspicommandmanager.cpp
  atspiconfiguration.cpp
  atspiscanner.cpp
)

ki18n_wrap_ui(simonatspicommandplugin_SRCS  atspiconfigurationdlg.ui)

kde4_add_library(simonatspicommandplugin MODULE ${simonatspicommandplugin_SRCS})

target_link_libraries(simonatspicommandplugin  Qt5::Xml
  ${QACCESSIBILITYCLIENT_LIBRARY} simonscenarios simonactions)

install(TARGETS simonatspicommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandatspiplugin)
install(FILES simonatspicommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandatspiplugin)

# add_subdirectory(test)
