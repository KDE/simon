include(FindQt4)

include(SimonDirs)

find_package(KDE4 REQUIRED)
find_package(Qt4 COMPONENTS QtNetwork)

set (QT_USE_QTNETWORK true)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES}
	../../simonlib)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DMAKE_SSCDACESS_LIB)




set( sscdaccess_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sscdaccess.cpp
)

set( sscdaccess_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/sscdaccess.h
)

kde4_add_library( sscdaccess  SHARED ${sscdaccess_LIB_SRCS} )


target_link_libraries( sscdaccess ${QT_QTCORE_LIBRARY}
	${QT_QTNETWORK_LIBRARY} ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} sscobjects )

set_target_properties( sscdaccess
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${sscdaccess_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscaccess
  COMPONENT simondevel
)
 
install( TARGETS sscdaccess DESTINATION ${SIMON_LIB_INSTALL_DIR}
	COMPONENT ssc )

