include(FindQt4)
include(FindPortaudio)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${PORTAUDIO_INCLUDE_DIR}
	../speechgui ../ ../../julius/libjulius/include ../ ../../julius/libsent/include )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)


IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

set( simonsound_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/wavrecorder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wavplayer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundcontrol.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wav.cpp
)

set( simonsound_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recwidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundsettings.h
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.h
	${CMAKE_CURRENT_SOURCE_DIR}/soundcontrol.h
)

kde4_add_kcfg_files(simonsound_LIB_SRCS soundconfig.kcfgc)

kde4_add_ui_files( simonsound_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/prompt.ui
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.ui
	${CMAKE_CURRENT_SOURCE_DIR}/deviceconfiguration.ui
)

kde4_add_library( simonsound  SHARED ${simonsound_LIB_SRCS} )

target_link_libraries( simonsound ${QT_LIBRARIES} ${PORTAUDIO_LIBRARIES}
${KDE4_KDEUI_LIBS} speechgui adinstreamer)


set_target_properties( simonsound
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonsound_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonsound
  COMPONENT simondevel
)
 
install( TARGETS simonsound  ${INSTALL_TARGETS_DEFAULT_ARGS} COMPONENT simon )
install( FILES simonsoundconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
