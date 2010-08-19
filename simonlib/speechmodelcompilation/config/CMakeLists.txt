include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonmodelcompilationconfiguration_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationsettings.cpp
)

kde4_add_ui_files( simonmodelcompilationconfiguration_LIB_SRCS
	externalprograms.ui
)

kde4_add_kcfg_files(simonmodelcompilationconfiguration_LIB_SRCS modelcompilationconfiguration.kcfgc)

kde4_add_plugin( simonmodelcompilationconfiguration  ${simonmodelcompilationconfiguration_LIB_SRCS} )
target_link_libraries( simonmodelcompilationconfiguration ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} )


set_target_properties( simonmodelcompilationconfiguration
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodelcompilationconfiguration_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelcompilation/config
  COMPONENT simonddevel
)
 
install( TARGETS simonmodelcompilationconfiguration DESTINATION
  ${PLUGIN_INSTALL_DIR} COMPONENT simoncore )
install( FILES simonmodelcompilationconfig.desktop  DESTINATION
  ${SERVICES_INSTALL_DIR} COMPONENT simoncore )
