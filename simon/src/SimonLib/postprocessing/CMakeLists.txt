include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( postprocessing_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/postprocessing.cpp
)

set( postprocessing_LIB_HDRS
  simonpostprocessing_export.h
  postprocessing.h
)


# kde4_add_kcfg_files(postprocessing_LIB_SRCS ../../coreconfiguration.kcfgc)

kde4_add_library( postprocessing SHARED ${postprocessing_LIB_SRCS} )
target_link_libraries( postprocessing ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )


set_target_properties( postprocessing
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${postprocessing_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/postprocessing
  COMPONENT Devel
)
 
install( TARGETS postprocessing  ${INSTALL_TARGETS_DEFAULT_ARGS} )