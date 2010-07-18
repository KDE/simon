find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simondictationcommandplugin_SRCS 
	dictationcommandmanager.cpp 
	dictationconfiguration.cpp
    )


kde4_add_ui_files(simondictationcommandplugin_SRCS 
  dictationconfigurationdlg.ui)

kde4_add_plugin(simondictationcommandplugin ${simondictationcommandplugin_SRCS})


target_link_libraries(simondictationcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simonscenarios eventsimulation)

install(TARGETS simondictationcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddictationplugin )
install( FILES simondictationcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddictationplugin )
add_subdirectory(po)
