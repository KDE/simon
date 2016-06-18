set (simondesktopgridplugin_SRCS 
  desktopgridcommandmanager.cpp
  desktopgridconfiguration.cpp 
  screengrid.cpp
)

ki18n_wrap_ui(simondesktopgridplugin_SRCS desktopgridconfigurationdlg.ui)

kde4_add_plugin(simondesktopgridplugin ${simondesktopgridplugin_SRCS})

target_link_libraries(simondesktopgridplugin 
  Qt5::Xml
  eventsimulation simonlogging simonscenarios simonactions)

install(TARGETS simondesktopgridplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin)
install(FILES simondesktopgridplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin)
