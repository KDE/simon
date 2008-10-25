include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../speechgui ../../../simonlib/ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonrecognition_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/networksettings.cpp
)

set( simonrecognition_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.h
)

kde4_add_kcfg_files(simonrecognition_LIB_SRCS recognitionconfiguration.kcfgc)

kde4_add_ui_files( simonrecognition_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/networksettingsdlg.ui
)

kde4_add_library( simonrecognitioncontrol  SHARED ${simonrecognition_LIB_SRCS} )

target_link_libraries( simonrecognitioncontrol ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} speechgui speechmodelbase
  simonmodelmanagement ) # for building the modelcontainers for synchronisation


set_target_properties( simonrecognitioncontrol
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonrecognition_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/RecognitionControl
  COMPONENT Devel
)
 
install( TARGETS simonrecognitioncontrol  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simonrecognitionconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
