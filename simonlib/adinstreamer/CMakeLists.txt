include(FindQt4)
include(FindJulius)
find_package(KDE4 REQUIRED)
find_package(Julius REQUIRED)
set (QT_USE_QTNETWORK true)

include(${QT_USE_FILE})
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${JULIUS_INCLUDE_DIR} ../ )

IF(UNIX)
	include(FindSoundFile)
	find_package(SoundFile REQUIRED)
ENDIF(UNIX)
IF(WIN32)
	include(FindDirectSound)
	find_package(DirectSound REQUIRED)
	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
ENDIF(WIN32)


add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( adinstreamer_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/adinstreamer.cpp
)



set( adinstreamer_LIB_HDRS
  adinstreamer_export.h
  adinstreamer.h
)

kde4_add_library( adinstreamer  SHARED ${adinstreamer_LIB_SRCS} )

IF(UNIX)
target_link_libraries( adinstreamer ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${JULIUS_LIBRARIES}
	${PORTAUDIO_LIBRARIES} ${SOUNDFILE_LIBRARY})
ENDIF(UNIX)

IF(WIN32)
target_link_libraries( adinstreamer ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} ${PORTAUDIO_LIBRARIES} ${DIRECTSOUND_LIBRARY}
	${JULIUS_LIBRARIES} wsock32 dsound winmm )
ENDIF(WIN32)


set_target_properties( adinstreamer
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${adinstreamer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/adinstreamer
  COMPONENT Devel
)
 
install( TARGETS adinstreamer  ${INSTALL_TARGETS_DEFAULT_ARGS} )
