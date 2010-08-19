include(UseQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonxml_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/xmldomreader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/xmlsaxreader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/xmlreader.cpp
)



set( simonxml_LIB_HDRS
  simonxml_export.h
  xmlreader.h
  xmldomreader.h
  xmlsaxreader.h
)


kde4_add_library( simonxml  SHARED ${simonxml_LIB_SRCS} )
target_link_libraries( simonxml ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS} )


set_target_properties( simonxml
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonxml_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonxml
  COMPONENT simondevel
)
 
install( TARGETS simonxml DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT
  simoncore )
