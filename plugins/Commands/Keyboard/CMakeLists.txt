find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonkeyboardcommandplugin_SRCS 
	keyboardcommandmanager.cpp 
	keyboardconfiguration.cpp
	keyboardtab.cpp
	keyboardset.cpp
	keyboardbutton.cpp
	keyboardsetxmlreader.cpp
    )

kde4_add_ui_files(simonkeyboardcommandplugin_SRCS 
	keyboardwidget.ui
	keyboardconfigurationdlg.ui
	keyboardaddbuttondlg.ui
    )

kde4_add_plugin(simonkeyboardcommandplugin ${simonkeyboardcommandplugin_SRCS})


target_link_libraries(simonkeyboardcommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
	simoncommandpluginbase simonactions eventsimulation simoninfo simonxml)

install(TARGETS simonkeyboardcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandkeyboardplugin )
install( FILES simonkeyboardcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandkeyboardplugin )
add_subdirectory(po)

install(FILES simonkeyboardpluginui.rc DESTINATION
	${DATA_INSTALL_DIR} COMPONENT simoncommandkeyboardplugin)
