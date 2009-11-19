include(FindQt4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)

find_package(KDE4 REQUIRED)

set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES}
	../../simonlib)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})


IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -D__NO_ISOCEXT -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

include(KDE4Defaults)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
include(UseQt4)

set( sscdaccess_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sscdaccess.cpp
)

set( sscdaccess_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/sscdaccess.h
)

kde4_add_library( sscdaccess  SHARED ${sscdaccess_LIB_SRCS} )


target_link_libraries( sscdaccess ${QT_LIBRARIES}
	${KDE4_KDEUI_LIBS} simonprogresstracking sscobjects )

set_target_properties( sscdaccess
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${sscdaccess_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscaccess
  COMPONENT simondevel
)
 
install( TARGETS sscdaccess DESTINATION ${SIMON_LIB_INSTALL_DIR}
	COMPONENT ssc )

