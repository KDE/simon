include(FindQt4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)
include(FindPortaudio)

find_package(KDE4 REQUIRED)

set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../../simonlib/ ../  ../../../julius/libjulius/include 
 ../../../julius/libsent/include)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DMAKE_RECOGNITIONCONTROL_LIB)


IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

include(KDE4Defaults)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
include(UseQt4)

set( simonrecognition_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/networksettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/synchronisationsettings.cpp
)

set( simonrecognition_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.h
)

kde4_add_kcfg_files(simonrecognition_LIB_SRCS recognitionconfiguration.kcfgc)

kde4_add_ui_files( simonrecognition_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/networksettingsdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/synchronisationsettings.ui
)

kde4_add_library( simonrecognitioncontrol  SHARED ${simonrecognition_LIB_SRCS} )


target_link_libraries( simonrecognitioncontrol ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} simondstreamer simonuicomponents
  simonmodelmanagementui simonscenarios simonrecognitionresult simonprogresstracking )

set_target_properties( simonrecognitioncontrol
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonrecognition_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/RecognitionControl
  COMPONENT simondevel
)
 
install( TARGETS simonrecognitioncontrol DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
install( FILES simonrecognitionconfig.desktop simonsynchronisationconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
