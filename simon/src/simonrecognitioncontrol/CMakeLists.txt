

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)



set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../../simonlib/ ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} )


IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

include(KDE4Defaults)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
include(UseQt4)

set( simonrecognition_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/threadedsslsocket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/simonsslsocket.cpp
)

set( simonrecognition_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.h
)

kde4_add_kcfg_files(simonrecognition_LIB_SRCS recognitionconfiguration.kcfgc)


kde4_add_library( simonrecognitioncontrol  SHARED ${simonrecognition_LIB_SRCS} )


target_link_libraries( simonrecognitioncontrol ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} simondstreamer
  simonmodelmanagementui simonscenarios simonrecognitionresult simonprogresstracking )

set_target_properties( simonrecognitioncontrol
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR} DEFINE_SYMBOL MAKE_RECOGNITIONCONTROL_LIB
)

install( FILES ${simonrecognition_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/RecognitionControl
  COMPONENT simondevel
)
 
install( TARGETS simonrecognitioncontrol DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )

#####

set( kcm_simonrecognitioncontrol_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/networksettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/synchronisationsettings.cpp
)

kde4_add_ui_files( kcm_simonrecognitioncontrol_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/networksettingsdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/synchronisationsettings.ui
)

kde4_add_kcfg_files(kcm_simonrecognitioncontrol_LIB_SRCS recognitionconfiguration.kcfgc)

kde4_add_plugin(simonrecognitioncontrolkcm ${kcm_simonrecognitioncontrol_LIB_SRCS})

target_link_libraries( simonrecognitioncontrolkcm  ${KDE4_KUTILS_LIBS} ${KDE4_KDEUI_LIBS}
	 simonuicomponents simonrecognitioncontrol
)

install(TARGETS simonrecognitioncontrolkcm DESTINATION ${PLUGIN_INSTALL_DIR}
  COMPONENT simon)

#simonsynchronisationconfig.desktop 
install( FILES simonrecognitionconfig.desktop DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
