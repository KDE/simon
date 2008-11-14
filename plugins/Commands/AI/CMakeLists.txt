find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_INSTALL_PREFIX}/include ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonaiplugin_SRCS 
	aicommandmanager.cpp
	aiconfiguration.cpp
	aimlparser.cpp
)

kde4_add_ui_files(simonaiplugin_SRCS aiconfigurationdlg.ui)

kde4_add_plugin(simonaiplugin ${simonaiplugin_SRCS})


target_link_libraries(simonaiplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY} ${QT_LIBRARIES} simonlogging simoncommandpluginbase)

install(TARGETS simonaiplugin DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES simonaiplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES simonaipluginconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES bot.xml vars.xml substitutions.xml  DESTINATION ${DATA_INSTALL_DIR}/ai/util )

add_subdirectory(aimls)
