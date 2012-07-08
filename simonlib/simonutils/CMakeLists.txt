include(UseQt4)
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} )
include(KDE4Defaults)

set( simonutils_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/fileutils.cpp
)



set( simonutils_LIB_HDRS
  simonutils_export.h
  fileutils.h
)


kde4_add_library( simonutils  SHARED ${simonutils_LIB_SRCS} )
target_link_libraries( simonutils ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( simonutils
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonutils_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonutils
  COMPONENT simondevel
)
 
install( TARGETS simonutils DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT
  simoncore )
