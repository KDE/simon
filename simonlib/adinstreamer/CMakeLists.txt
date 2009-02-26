include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
set (QT_USE_QTNETWORK true)

include(${QT_USE_FILE})
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../julius/libsent/include/ ../../julius/libjulius/include/ ../ )

IF(WIN32)
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

target_link_libraries( adinstreamer ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} ${PORTAUDIO_LIBRARIES} julius sent )


set_target_properties( adinstreamer
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${adinstreamer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/adinstreamer
  COMPONENT simondevel
)
 
install( TARGETS adinstreamer DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
install( FILES adin.jconf DESTINATION ${DATA_INSTALL_DIR}/simon COMPONENT simon )
