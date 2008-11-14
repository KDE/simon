include(FindQt4)
find_package(KDE4 REQUIRED)
set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../../simonlib/ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simondconfiguration_LIB_SRCS
	#${CMAKE_CURRENT_SOURCE_DIR}/simonduserconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simondnetworkconfiguration.cpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simondconfigurationmain.cpp
)

kde4_add_kcfg_files(simondconfiguration_LIB_SRCS simondconfiguration.kcfgc)

kde4_add_ui_files( simondconfiguration_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simondnetworkconfiguration.ui
	${CMAKE_CURRENT_SOURCE_DIR}/simonduserconfiguration.ui
)

kde4_add_library( simondconfiguration SHARED ${simondconfiguration_LIB_SRCS} )

target_link_libraries( simondconfiguration ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} speechgui )

set_target_properties( simondconfiguration
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

 
install( TARGETS simondconfiguration  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simondnetworkconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES simonduserconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
