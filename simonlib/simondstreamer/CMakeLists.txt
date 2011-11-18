find_package(Qt4 COMPONENTS QtCore QtDbus)
include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)

#set (QT_USE_QTNETWORK true)
#include(${QT_USE_FILE})

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
	${QT_MOBILITY_MULTIMEDIAKIT_INCLUDE_DIR} ../)

#IF(WIN32)
#	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
#ENDIF(WIN32)


add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simondstreamer_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simondstreamer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simondstreamerclient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonsender.cpp
)


set( simondstreamer_LIB_HDRS
  simondstreamer_export.h
  simondstreamer.h
  simonsender.h
)

qt4_add_dbus_adaptor( simondstreamer_LIB_SRCS org.simon-listens.SimonSender.xml
  simonsender.h SimonSender)

kde4_add_library( simondstreamer  SHARED ${simondstreamer_LIB_SRCS} )

target_link_libraries( simondstreamer ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
	${QT_MOBILITY_MULTIMEDIAKIT_LIBRARY}
	simonsound simonwav )


set_target_properties( simondstreamer
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simondstreamer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondstreamer
  COMPONENT simondevel
)
 
install( TARGETS simondstreamer DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
