include(FindQt4)
include(SimonDirs)
find_package(KDE4 REQUIRED)
set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../../simonlib/ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simondsettings_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simonduserconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simondnetworkconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
)

kde4_add_kcfg_files(simondsettings_LIB_SRCS simondconfiguration.kcfgc)

kde4_add_ui_files( simondsettings_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simondnetworkconfiguration.ui
	${CMAKE_CURRENT_SOURCE_DIR}/simonduserconfiguration.ui
)

kde4_add_plugin( simondsettings ${simondsettings_LIB_SRCS} )

target_link_libraries( simondsettings ${QT_LIBRARIES} ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonddatabaseaccess
	simonuicomponents )
 
install( TARGETS simondsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simond )
install( FILES simondconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simond )
