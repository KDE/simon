include(FindQt4)
include(SimonDirs)
find_package(KDE4 REQUIRED)
set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../../simonlib/ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( sscdsettings_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sscduserconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sscdnetworkconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
)

kde4_add_kcfg_files(sscdsettings_LIB_SRCS sscdconfiguration.kcfgc)

kde4_add_ui_files( sscdsettings_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sscdnetworkconfiguration.ui
	${CMAKE_CURRENT_SOURCE_DIR}/sscduserconfiguration.ui
)

kde4_add_library( sscdsettings SHARED ${sscdsettings_LIB_SRCS} )

target_link_libraries( sscdsettings ${QT_LIBRARIES} ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} speechgui simonddatabaseaccess )

set_target_properties( sscdsettings
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

 
install( TARGETS sscdsettings DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT sscd )
install( FILES sscdnetworkconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT sscd )
install( FILES sscduserconfiguration.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT sscd )
