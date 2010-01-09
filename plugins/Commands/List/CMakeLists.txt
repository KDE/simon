find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonlistcommandplugin_SRCS 
	createlistcommandwidget.cpp 
	listcommandmanager.cpp 
	launchercommand.cpp 
    )

kde4_add_ui_files(simonlistcommandplugin_SRCS 
	createlistcommandwidget.ui
    )

kde4_add_plugin(simonlistcommandplugin ${simonlistcommandplugin_SRCS})


target_link_libraries(simonlistcommandplugin ${KDE4_KDEUI_LIBS}
	${KDE4_KIO_LIBS} simonscenarios 
	simonlogging simonactions)

install(TARGETS simonlistcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandlistplugin )
install( FILES simonlistcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandlistplugin )
add_subdirectory(po)
