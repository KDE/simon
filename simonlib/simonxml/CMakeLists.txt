include(UseQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

#set (QT_USE_QTMAIN true)
#set (QT_USE_XML true)
#include(${QT_USE_FILE})

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

install( FILES ${simoninfo_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonxml
  COMPONENT Devel
)
 
install( TARGETS simonxml ${INSTALL_TARGETS_DEFAULT_ARGS} )
