include(FindQt4)
include(SimonDirs)

find_package(Qt4 COMPONENTS QtCore QtXml QtGui REQUIRED)
include(${QT_USE_FILE})

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonttsconfiguration_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/ttssettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/editrecording.cpp
)

kde4_add_ui_files( simonttsconfiguration_LIB_SRCS
	simonttsconfigurationdlg.ui
	editrecording.ui
)

kde4_add_kcfg_files(simonttsconfiguration_LIB_SRCS ttsconfiguration.kcfgc)

kde4_add_plugin( simonttsconfiguration  ${simonttsconfiguration_LIB_SRCS} )
target_link_libraries( simonttsconfiguration simonsound simontts 
  ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} )

set_target_properties( simonttsconfiguration
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( TARGETS simonttsconfiguration DESTINATION
  ${PLUGIN_INSTALL_DIR} COMPONENT simoncore )
install( FILES simonttsconfig.desktop  DESTINATION
  ${SERVICES_INSTALL_DIR} COMPONENT simoncore )
