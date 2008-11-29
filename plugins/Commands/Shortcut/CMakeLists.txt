find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

IF(WIN32)
add_definitions(-D__INTERLOCKED_DECLARED)
ENDIF(WIN32)

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonshortcutcommandplugin_SRCS 
	createshortcutcommandwidget.cpp 
	shortcutcommand.cpp 
	shortcutcommandmanager.cpp 
	xmlshortcutcommand.cpp
    )

kde4_add_ui_files(simonshortcutcommandplugin_SRCS 
	createshortcutcommandwidget.ui
    )

kde4_add_plugin(simonshortcutcommandplugin ${simonshortcutcommandplugin_SRCS})


target_link_libraries(simonshortcutcommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simoncommandpluginbase eventsimulation simonxml simonlogging)

install(TARGETS simonshortcutcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES simonshortcutcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )

add_subdirectory(po)
