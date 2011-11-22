
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)
 
set( simonjsonconnector_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simonjsonconnector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/jsonnotificationreceiver.cpp
)

set( simonjsonconnector_LIB_HDRS
  simonjsonconnector_export.h
  simonjsonconnector.h
)

kde4_add_library( simonjsonconnector  SHARED ${simonjsonconnector_LIB_SRCS} )
target_link_libraries( simonjsonconnector ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS}
  ${KDE4_KIO_LIBS} )


set_target_properties( simonjsonconnector
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonjsonconnector_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/jsonconnector
  COMPONENT simoncoredevel
)
 
install( TARGETS simonjsonconnector DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
