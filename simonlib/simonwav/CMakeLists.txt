include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES})

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonwav_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/wav.cpp
)

set( simonwav_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/wav.h
)

kde4_add_library( simonwav  SHARED ${simonwav_LIB_SRCS} )


target_link_libraries( simonwav ${QT_QTCORE_LIBRARY} )


set_target_properties( simonwav
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonwav_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonwav
  COMPONENT simondevel
)
 
install( TARGETS simonwav DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT
	simoncore )
