include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DDEBUG -DQT_DEBUG)
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonprogresstracking_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/operation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/statusmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/progresswidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/compositeprogresswidget.cpp
)



set( simonprogresstracking_LIB_HDRS
  simonprogresstracking_export.h
  statusmanager.h
  operation.h
)


kde4_add_library( simonprogresstracking  SHARED ${simonprogresstracking_LIB_SRCS} )
target_link_libraries( simonprogresstracking ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )


set_target_properties( simonprogresstracking
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonprogresstracking_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonprogresstracking
  COMPONENT simondevel
)
 
install( TARGETS simonprogresstracking DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
