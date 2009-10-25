find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
IF(WIN32)
add_definitions(-D__INTERLOCKED_DECLARED)
ENDIF(WIN32)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})


include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonshortcutcommandplugin_SRCS 
	createshortcutcommandwidget.cpp 
	shortcutcommand.cpp 
	shortcutcommandmanager.cpp 
    )

kde4_add_ui_files(simonshortcutcommandplugin_SRCS 
	createshortcutcommandwidget.ui
    )

kde4_add_plugin(simonshortcutcommandplugin ${simonshortcutcommandplugin_SRCS})

target_link_libraries(simonshortcutcommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_LIBRARIES} simonscenarios eventsimulation simonlogging)

install(TARGETS simonshortcutcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandshortcutplugin )
install( FILES simonshortcutcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandshortcutplugin )

add_subdirectory(po)
