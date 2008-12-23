find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simondesktopgridplugin_SRCS 
	desktopgridcommandmanager.cpp
	desktopgridconfiguration.cpp 
	screengrid.cpp
)

kde4_add_ui_files(simondesktopgridplugin_SRCS desktopgridconfigurationdlg.ui)

kde4_add_plugin(simondesktopgridplugin ${simondesktopgridplugin_SRCS})


target_link_libraries(simondesktopgridplugin ${KDE4_KDEUI_LIBS} eventsimulation simonlogging simoncommandpluginbase)

install(TARGETS simondesktopgridplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin )
install( FILES simondesktopgridplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin )
install( FILES simondesktopgridpluginconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddesktopgridplugin )
add_subdirectory(po)
