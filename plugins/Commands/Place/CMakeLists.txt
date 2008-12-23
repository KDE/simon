find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonplacecommandplugin_SRCS 
	createplacecommandwidget.cpp 
	placecommand.cpp 
	placecommandmanager.cpp 
	xmlplacecommand.cpp 
	ImportPlace/importplacewizard.cpp
	ImportPlace/selectplacepage.cpp
    )

kde4_add_ui_files(simonplacecommandplugin_SRCS 
	ImportPlace/selectplacedlg.ui
	createplacecommandwidget.ui
    )

kde4_add_plugin(simonplacecommandplugin ${simonplacecommandplugin_SRCS})


target_link_libraries(simonplacecommandplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simoncommandpluginbase simonxml simonlogging)

install(TARGETS simonplacecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandplaceplugin )
install( FILES simonplacecommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandplaceplugin )
