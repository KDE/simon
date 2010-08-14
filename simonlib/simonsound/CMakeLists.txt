include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

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
)

set( simonsound_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recwidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/volumewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/devicevolumewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundclient.h
# 	${CMAKE_CURRENT_SOURCE_DIR}/soundsettings.h
	${CMAKE_CURRENT_SOURCE_DIR}/devicesettings.h
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundserver.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundinputclient.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsound.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsoundinput.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsamples.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonsoundoutput.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundoutputclient.h
	${CMAKE_CURRENT_SOURCE_DIR}/wavfilewidget.h

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


target_link_libraries( simonsound ${QT_LIBRARIES}
	${QT_QTMULTIMEDIA_LIBRARY} 
		${KDE4_KDEUI_LIBS} simonwav)


set_target_properties( simonsound
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonsound_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonsound
  COMPONENT simondevel
)
 
install( TARGETS simonsound DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )


############

set( kcm_simonsound_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/soundsettings.cpp
)

kde4_add_kcfg_files(kcm_simonsound_LIB_SRCS soundconfig.kcfgc)

kde4_add_plugin(simonsoundkcm ${kcm_simonsound_LIB_SRCS})
target_link_libraries( simonsoundkcm  ${KDE4_KDEUI_LIBS} 
simonsound
)

install(TARGETS simonsoundkcm DESTINATION ${PLUGIN_INSTALL_DIR} )

install( FILES simonsoundconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
