set (simondesktopgridplugin_SRCS 
  desktopgridcommandmanager.cpp
  desktopgridconfiguration.cpp 
  screengrid.cpp
)

kde4_add_ui_files(simondesktopgridplugin_SRCS desktopgridconfigurationdlg.ui)

kde4_add_plugin(simondesktopgridplugin ${simondesktopgridplugin_SRCS})

target_link_libraries(simondesktopgridplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simonscenariobase
  eventsimulation simonlogging simonscenarios simonactions)

install(TARGETS simondesktopgridplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin)
install(FILES simondesktopgridplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin)
