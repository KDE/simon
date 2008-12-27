include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTSQL true)
include(${QT_USE_FILE})

set( simonddatabaseaccess_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/databaseaccess.cpp
)



set( simonddatabaseaccess_LIB_HDRS
  simonddatabaseaccess_export.h
  databaseaccess.h
)


kde4_add_library( simonddatabaseaccess  SHARED ${simonddatabaseaccess_LIB_SRCS} )
target_link_libraries( simonddatabaseaccess ${KDE4_KDECORE_LIBS}
	${QT_LIBRARIES} )


set_target_properties( simonddatabaseaccess
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonddatabaseaccess_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simond/simonddatabaseaccess
  COMPONENT simonddevel
)
 
install( TARGETS simonddatabaseaccess DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )
