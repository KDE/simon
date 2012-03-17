include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../
                        ${CMAKE_CURRENT_SOURCE_DIR}/..
                        ${CMAKE_CURRENT_SOURCE_DIR}/../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)
 
set( simoncontextadapter_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/contextadapter.cpp
)

set( simoncontextadapter_LIB_HDRS
  simoncontextadapter_export.h
  contextadapter.h
)

kde4_add_library( simoncontextadapter  SHARED ${simoncontextadapter_LIB_SRCS} )
target_link_libraries( simoncontextadapter ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} 
        ${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS} simonmodelcompilationadapter
        simonmodelcompilation )


set_target_properties( simoncontextadapter
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simoncontextdetection_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/contextadapter
  COMPONENT simonddevel
)
 
install( TARGETS simoncontextadapter DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )
