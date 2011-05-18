include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)
 
set( simoncontextdetection_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/contextmanager.cpp
)

set( simoncontextdetection_LIB_HDRS
  simoncontextdetection_export.h
  contextmanager.h
)

kde4_add_library( simoncontextdetection  SHARED ${simoncontextdetection_LIB_SRCS} )
target_link_libraries( simoncontextdetection ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( simoncontextdetection
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simoncontextdetection_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/logging
  COMPONENT simoncoredevel
)
 
install( TARGETS simoncontextdetection DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )