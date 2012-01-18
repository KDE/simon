
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonrecognizercommandplugin_SRCS 
	recognizercommandmanager.cpp 
	recognizerconfiguration.cpp
    )


kde4_add_ui_files(simonrecognizercommandplugin_SRCS 
  recognizerconfigurationdlg.ui)

kde4_add_plugin(simonrecognizercommandplugin ${simonrecognizercommandplugin_SRCS})


target_link_libraries(simonrecognizercommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simonscenarios eventsimulation simonscenariobase)

install(TARGETS simonrecognizercommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandrecognizerplugin )
install( FILES simonrecognizercommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandrecognizerplugin )
