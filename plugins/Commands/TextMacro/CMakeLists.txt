
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simontextmacrocommandplugin_SRCS 
	createtextmacrocommandwidget.cpp 
	textmacrocommand.cpp 
	textmacrocommandmanager.cpp 
    )

kde4_add_ui_files(simontextmacrocommandplugin_SRCS 
	createtextmacrocommandwidget.ui
    )

kde4_add_plugin(simontextmacrocommandplugin ${simontextmacrocommandplugin_SRCS})


target_link_libraries(simontextmacrocommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonscenarios simonscenariobase eventsimulation simonlogging)

install(TARGETS simontextmacrocommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandtextmacroplugin )
install( FILES simontextmacrocommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandtextmacroplugin )
