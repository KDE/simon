find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonexecutablecommandplugin_SRCS 
	createexecutablecommandwidget.cpp 
	executablecommand.cpp 
	executablecommandmanager.cpp 
	xmlexecutablecommand.cpp 
	ImportProgram/importprogramwizard.cpp
	ImportProgram/selectprogrampage.cpp
    )

kde4_add_ui_files(simonexecutablecommandplugin_SRCS 
	ImportProgram/selectprogramdlg.ui
	createexecutablecommandwidget.ui
    )

kde4_add_plugin(simonexecutablecommandplugin ${simonexecutablecommandplugin_SRCS})


target_link_libraries(simonexecutablecommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simoncommandpluginbase simonxml simonlogging)

install(TARGETS simonexecutablecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES simonexecutablecommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
add_subdirectory(po)
