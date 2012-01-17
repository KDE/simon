
include(KDE4Defaults)
include(KDE4Macros)

include_directories(
  ${KDE4_INCLUDE_DIR}
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib
)

set (simoncalculatorplugin_SRCS 
	calculatorcommandmanager.cpp
	token.cpp
	calculatorconfiguration.cpp
)

kde4_add_ui_files(simoncalculatorplugin_SRCS calculatorconfigurationdlg.ui calculatorwidget.ui)

kde4_add_plugin(simoncalculatorplugin ${simoncalculatorplugin_SRCS})

target_link_libraries(simoncalculatorplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
	eventsimulation simonlogging simonscenarios simonactions
	simoninfo simonscenariobase)

install(TARGETS simoncalculatorplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin )
install( FILES simoncalculatorcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcalculatorplugin )
add_subdirectory(po)
