include(SimonDirs)

find_package(LibSampleRate)


IF(UNIX)
  find_package(ALSA REQUIRED)
  set (plattformLibraries ${ALSA_LIBRARY})
  include_directories ( ${ALSA_INCLUDE_DIR} )
ELSE(UNIX)
  find_package(DirectSound REQUIRED)
  set (plattformLibraries ${DIRECTSOUND_LIBRARY})
  include_directories ( ${DIRECTSOUND_INCLUDE_DIRS} )
ENDIF(UNIX)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
  ${QT_MOBILITY_MULTIMEDIAKIT_INCLUDE_DIR} ../)

if(LIBSAMPLERATE_FOUND)
include_directories(${LIBSAMPLERATE_INCLUDE_DIR})
add_definitions(${LIBSAMPLERATE_DEFINITIONS})
endif(LIBSAMPLERATE_FOUND)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

set( simonsound_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/recwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/volumewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/devicevolumewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/soundserver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundinputclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundoutputclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wavrecorderclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/nullrecorderclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wavplayerclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wavplayersubclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wavfilewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundprocessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsoundinput.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonsoundoutput.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/loudnessmetersoundprocessor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vadsoundprocessor.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/trainsamplevolumepage.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/devicesettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/singledevicesettings.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/soundbuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundinputbuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundoutputbuffer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundbackend.cpp
	
  ${CMAKE_CURRENT_SOURCE_DIR}/qsemaphore2.cpp
)

IF(WIN32)
  set ( simonsound_LIB_SRCS ${simonsound_LIB_SRCS} 
	${CMAKE_CURRENT_SOURCE_DIR}/directsound/directsoundbackend.cpp
  )
ENDIF(WIN32)
IF(UNIX)
  set ( simonsound_LIB_SRCS ${simonsound_LIB_SRCS} 
	${CMAKE_CURRENT_SOURCE_DIR}/alsa/alsabackend.cpp
  )
ENDIF(UNIX)

if(LIBSAMPLERATE_FOUND)
  set ( simonsound_LIB_SRCS ${simonsound_LIB_SRCS} 
	${CMAKE_CURRENT_SOURCE_DIR}/resamplesoundprocessor.cpp
  )
endif(LIBSAMPLERATE_FOUND)

set( simonsound_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recwidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/devicesettings.h
	${CMAKE_CURRENT_SOURCE_DIR}/volumewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/devicevolumewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundclient.h
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundserver.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundinputclient.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsound.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsoundinput.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsamples.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsoundoutput.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundoutputclient.h
	${CMAKE_CURRENT_SOURCE_DIR}/wavplayerclient.h
	${CMAKE_CURRENT_SOURCE_DIR}/wavfilewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundbackendclient.h

	${CMAKE_CURRENT_SOURCE_DIR}/simonsound_export.h

	${CMAKE_CURRENT_SOURCE_DIR}/soundprocessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/loudnessmetersoundprocessor.h
	${CMAKE_CURRENT_SOURCE_DIR}/vadsoundprocessor.h
	
	${CMAKE_CURRENT_SOURCE_DIR}/trainsamplevolumepage.h
)

kde4_add_kcfg_files(simonsound_LIB_SRCS soundconfig.kcfgc)

kde4_add_ui_files( simonsound_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/prompt.ui
	${CMAKE_CURRENT_SOURCE_DIR}/vad.ui
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.ui
	${CMAKE_CURRENT_SOURCE_DIR}/recwidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/volumewidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/devicevolumewidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/wavfilewidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/trainsamplevolumepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/trainingsettings.ui
)

kde4_add_ui_files(simonsound_LIB_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/deviceconfiguration.ui
	${CMAKE_CURRENT_SOURCE_DIR}/singledeviceconfiguration.ui)

kde4_add_library( simonsound  SHARED ${simonsound_LIB_SRCS} )


if(LIBSAMPLERATE_FOUND)
  message(STATUS "Enabling resample support")
target_link_libraries( simonsound ${QT_LIBRARIES} ${plattformLibraries}
  simonlogging simonuicomponents simoncontextcoreui simoncontextdetection
  simonwav ${QT_QTXML_LIBRARY}
  ${LIBSAMPLERATE_LIBRARY}
  ${KDE4_KDEUI_LIBS}
)
else(LIBSAMPLERATE_FOUND)
  message(STATUS "Disabling resample support")
target_link_libraries( simonsound ${QT_LIBRARIES} ${plattformLibraries}
  simonlogging simonuicomponents simoncontextcoreui simoncontextdetection
  simonwav ${QT_QTXML_LIBRARY}
  ${KDE4_KDEUI_LIBS})
endif(LIBSAMPLERATE_FOUND)

set_target_properties( simonsound
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonsound_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonsound
  COMPONENT simondevel
)
 
install( TARGETS simonsound DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT
  simoncore )


############

set( kcm_simonsound_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/soundsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/samplegroupcontextsettings.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/samplegroupitemdelegate.cpp
)

kde4_add_ui_files( kcm_simonsound_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/samplegroupcontextsettings.ui
	)
kde4_add_kcfg_files(kcm_simonsound_LIB_SRCS soundconfig.kcfgc)

kde4_add_plugin(simonsoundkcm ${kcm_simonsound_LIB_SRCS})
target_link_libraries( simonsoundkcm  ${KDE4_KDEUI_LIBS} 
simonsound simonuicomponents simoncontextcoreui simoncontextdetection
simonscenarios ${QT_QTXML_LIBRARY}
)

install(TARGETS simonsoundkcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT
  simoncore)

install( FILES simonsoundconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}
  COMPONENT simoncore )
