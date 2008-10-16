include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonmodelcompilation_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationsettings.cpp
)


set( simonmodelcompilation_LIB_HDRS
  simonmodelcompilationmanagement_export.h
  modelcompilationmanager.h
)



kde4_add_ui_files( simonmodelcompilation_LIB_SRCS
	externalprograms.ui
)

kde4_add_kcfg_files(simonmodelcompilation_LIB_SRCS modelcompilationconfiguration.kcfgc)

kde4_add_library( simonmodelcompilation  SHARED ${simonmodelcompilation_LIB_SRCS} )
target_link_libraries( simonmodelcompilation ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonlogging )


set_target_properties( simonmodelcompilation
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodelcompilation_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/modelcompilation
  COMPONENT Devel
)
 
install( TARGETS simonmodelcompilation  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simonmodelcompilationconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )