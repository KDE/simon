
include(SimonDirs)

set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../../simonlib/ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( ksimondsettings_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/ksimondsettings.cpp
)

kde4_add_kcfg_files(ksimondsettings_LIB_SRCS ksimondconfiguration.kcfgc)

kde4_add_ui_files( ksimondsettings_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/ksimondconfiguration.ui
)

kde4_add_plugin( ksimondsettings ${ksimondsettings_LIB_SRCS} )

target_link_libraries( ksimondsettings ${QT_LIBRARIES} ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} )
 
install( TARGETS ksimondsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT ksimond)
install( FILES ksimondconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT ksimond)
