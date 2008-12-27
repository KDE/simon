include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simoninfo_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simoninfo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/osd.cpp
)



set( simoninfo_LIB_HDRS
  simoninfo_export.h
  simoninfo.h
)


kde4_add_library( simoninfo  SHARED ${simoninfo_LIB_SRCS} )
target_link_libraries( simoninfo ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )


set_target_properties( simoninfo
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simoninfo_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simoninfo
  COMPONENT simoncore
)
 
install( TARGETS simoninfo DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )

add_subdirectory(themes)
