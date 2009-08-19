find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simoncalculatorplugin_SRCS 
	calculatorcommandmanager.cpp
	token.cpp
	calculatorconfiguration.cpp
)

kde4_add_ui_files(simoncalculatorplugin_SRCS calculatorconfigurationdlg.ui calculatorwidget.ui)

kde4_add_plugin(simoncalculatorplugin ${simoncalculatorplugin_SRCS})


target_link_libraries(simoncalculatorplugin ${KDE4_KDEUI_LIBS}
	eventsimulation simonlogging simoncommandpluginbase simonactions)

install(TARGETS simoncalculatorplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin )
install( FILES simoncalculatorcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin )
install( FILES simoncalculatorpluginconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin )
add_subdirectory(po)
