
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simondbuscommandplugin_SRCS 
	dbuscommandmanager.cpp 
	dbuscommand.cpp
	createdbuscommandwidget.cpp
    )

kde4_add_ui_files(simondbuscommandplugin_SRCS 
	createdbuscommandwidget.ui
    )

kde4_add_plugin(simondbuscommandplugin ${simondbuscommandplugin_SRCS})


target_link_libraries(simondbuscommandplugin ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY}
	simonscenarios simonscenariobase)

install(TARGETS simondbuscommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddbusplugin )
install( FILES simondbuscommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddbusplugin )
#add_subdirectory(po)

