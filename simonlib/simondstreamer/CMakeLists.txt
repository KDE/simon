include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)

#set (QT_USE_QTNETWORK true)
#include(${QT_USE_FILE})

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

#IF(WIN32)
#	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
#ENDIF(WIN32)


add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simondstreamer_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simondstreamer.cpp
)


set( simondstreamer_LIB_HDRS
  simondstreamer_export.h
  simondstreamer.h
)

kde4_add_library( simondstreamer  SHARED ${simondstreamer_LIB_SRCS} )

target_link_libraries( simondstreamer ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
	simonsound simonwav )


set_target_properties( simondstreamer
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simondstreamer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondstreamer
  COMPONENT simondevel
)
 
install( TARGETS simondstreamer DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
