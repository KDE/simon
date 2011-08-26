include(FindQt4)
find_package(Qt4 COMPONENTS QtConcurrent  QtNetwork)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)

find_package(KDE4 REQUIRED)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonsampleshare_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sampleshare.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonsampledataprovider.cpp
)

set( simonsampleshare_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/sampleshare.h)

kde4_add_ui_files( simonsampleshare_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sampleshare.ui)

kde4_add_library( simonsampleshare  SHARED ${simonsampleshare_LIB_SRCS} )

target_link_libraries( simonsampleshare ${QT_LIBRARIES} sscdaccess
${KDE4_KDEUI_LIBS} sscobjects simonscenarios simonuicomponents simonprogresstracking)


set_target_properties( simonsampleshare
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonsampleshare_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonsampleshare
  COMPONENT simondevel
)
 
install( TARGETS simonsampleshare DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
