find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simoncompositecommandplugin_SRCS 
	createcompositecommandwidget.cpp 
	compositecommand.cpp 
	delaycommand.cpp 
	compositecommandmanager.cpp 
    )

kde4_add_ui_files(simoncompositecommandplugin_SRCS 
	createcompositecommandwidget.ui
    )

kde4_add_plugin(simoncompositecommandplugin ${simoncompositecommandplugin_SRCS})


target_link_libraries(simoncompositecommandplugin ${KDE4_KDEUI_LIBS}
	${KDE4_KIO_LIBS} simonscenarios simonactions simonlogging)

install(TARGETS simoncompositecommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandcompositeplugin )
install( FILES simoncompositecommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandcompositeplugin )
add_subdirectory(po)
