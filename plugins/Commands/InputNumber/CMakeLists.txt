find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simoninputnumbercommandplugin_SRCS 
	inputnumbercommandmanager.cpp 
	inputnumberconfiguration.cpp 
    )

kde4_add_ui_files(simoninputnumbercommandplugin_SRCS 
	inputnumberwidget.ui
	inputnumberconfigurationdlg.ui
    )

kde4_add_plugin(simoninputnumbercommandplugin ${simoninputnumbercommandplugin_SRCS})


target_link_libraries(simoninputnumbercommandplugin ${KDE4_KDEUI_LIBS}
	simoncommandpluginbase simonactions eventsimulation)

install(TARGETS simoninputnumbercommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandinputnumberplugin )
install( FILES simoninputnumbercommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandinputnumberplugin )
add_subdirectory(po)
