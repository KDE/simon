include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simongreedyreceiver_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/greedyreceiver.cpp
)

set( simongreedyreceiver_LIB_HDRS
  simongreedyreceiver_export.h
  greedyreceiver.h
)


kde4_add_library( simongreedyreceiver  SHARED
	${simongreedyreceiver_LIB_SRCS} )
target_link_libraries( simongreedyreceiver ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
	simonactions simonrecognitionresult )


set_target_properties( simongreedyreceiver
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simongreedyreceiver_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/greedyreceiver
  COMPONENT simondevel
)

install( TARGETS simongreedyreceiver DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
