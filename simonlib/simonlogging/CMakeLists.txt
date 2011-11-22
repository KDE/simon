
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)
 
set( simonlogging_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
)

set( simonlogging_LIB_HDRS
  simonlogging_export.h
  logger.h
)

kde4_add_library( simonlogging  SHARED ${simonlogging_LIB_SRCS} )
target_link_libraries( simonlogging ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( simonlogging
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonlogging_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/logging
  COMPONENT simoncoredevel
)
 
install( TARGETS simonlogging DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )