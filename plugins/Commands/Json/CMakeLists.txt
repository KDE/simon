find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonjsoncommandplugin_SRCS 
	jsoncommandmanager.cpp 
  jsonconfiguration.cpp
	jsoncommand.cpp
	createjsoncommandwidget.cpp
    )

kde4_add_ui_files(simonjsoncommandplugin_SRCS 
  jsonconfigurationdlg.ui
	createjsoncommandwidget.ui
    )

kde4_add_plugin(simonjsoncommandplugin ${simonjsoncommandplugin_SRCS})


target_link_libraries(simonjsoncommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
	simonscenarios )

install(TARGETS simonjsoncommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandjsonplugin )
install( FILES simonjsoncommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandjsonplugin )
add_subdirectory(po)

