include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simoncommandpluginbase_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/createcommandwidget.cpp
)

set( simoncommandpluginbase_LIB_HDRS
  simoncommandpluginbase_export.h
  command.h
  commandmanager.h
  commandconfiguration.h
  createcommandwidget.h
)


kde4_add_library( simoncommandpluginbase  SHARED ${simoncommandpluginbase_LIB_SRCS} )
target_link_libraries( simoncommandpluginbase ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} simoninfo )


set_target_properties( simoncommandpluginbase
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simoncommandpluginbase_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/commandpluginbase
  COMPONENT simondevel
)

install(FILES
${CMAKE_CURRENT_SOURCE_DIR}/simoncommandplugin.desktop DESTINATION
${SERVICETYPES_INSTALL_DIR} COMPONENT simon)
 
install( TARGETS simoncommandpluginbase  ${INSTALL_TARGETS_DEFAULT_ARGS} COMPONENT simon )