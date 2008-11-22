include(FindQt4)
include(FindPortaudio)
include(FindJulius)

find_package(Julius REQUIRED)
find_package(KDE4 REQUIRED)

IF(UNIX)
	include(FindSoundFile)
	find_package(SoundFile REQUIRED)
ENDIF(UNIX)
IF(WIN32)
	include(FindDirectSound)
	find_package(DirectSound REQUIRED)
ENDIF(WIN32)

set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../speechgui
	../../../simonlib/ ../ ${JULIUS_INCLUDE_DIR} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DDEBUG -DQT_DEBUG)
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonrecognition_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/networksettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/adinstreamer.cpp
)

set( simonrecognition_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitioncontrol.h
)

kde4_add_kcfg_files(simonrecognition_LIB_SRCS recognitionconfiguration.kcfgc)

kde4_add_ui_files( simonrecognition_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/networksettingsdlg.ui
)

kde4_add_library( simonrecognitioncontrol  SHARED ${simonrecognition_LIB_SRCS} )


IF(UNIX)
target_link_libraries( simonrecognitioncontrol ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${PORTAUDIO_LIBRARIES} ${SOUNDFILE_LIBRARY} ${JULIUS_LIBRARIES} speechgui speechmodelbase
  simonmodelmanagementui ) # for building the modelcontainers for synchronisation
ENDIF(UNIX)
IF(WIN32)
target_link_libraries( simonrecognitioncontrol ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${PORTAUDIO_LIBRARIES} ${DIRECTSOUND_LIBRARY} ${JULIUS_LIBRARIES} speechgui speechmodelbase
  simonmodelmanagementui wsock32 dsound winmm ) # for building the modelcontainers for synchronisation
ENDIF(WIN32)

set_target_properties( simonrecognitioncontrol
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonrecognition_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/RecognitionControl
  COMPONENT Devel
)
 
install( TARGETS simonrecognitioncontrol  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simonrecognitionconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
