include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../Commands )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonactions_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/actionmanager.cpp
)

set( simonactions_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/commandmodel.h
	${CMAKE_CURRENT_SOURCE_DIR}/actionmanager.h
)

kde4_add_kcfg_files(simonactions_LIB_SRCS actionconfig.kcfgc)

kde4_add_ui_files( simonactions_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandsettingsdlg.ui
)

kde4_add_library( simonactions  SHARED ${simonactions_LIB_SRCS} )

target_link_libraries( simonactions ${QT_LIBRARIES}
${KDE4_KDEUI_LIBS} speechgui simoncommandpluginbase )


set_target_properties( simonactions
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonactions_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonactions
  COMPONENT Devel
)
 
install( TARGETS simonactions  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simonactionsconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
