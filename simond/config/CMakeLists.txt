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

kde4_add_library( simondsettings SHARED ${simondsettings_LIB_SRCS} )

target_link_libraries( simondsettings ${QT_LIBRARIES} ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonddatabaseaccess
	simonuicomponents )

set_target_properties( simondsettings
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

 
install( TARGETS simondsettings DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )
install( FILES simondnetworkconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simond )
install( FILES simonduserconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simond )
