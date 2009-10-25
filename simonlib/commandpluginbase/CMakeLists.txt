include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonscenarios_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/createcommandwidget.cpp
)

set( simonscenarios_LIB_HDRS
  simonscenarios_export.h
  command.h
  commandmanager.h
  commandconfiguration.h
  createcommandwidget.h
)


kde4_add_library( simonscenarios  SHARED
	${simonscenarios_LIB_SRCS} )
target_link_libraries( simonscenarios ${QT_LIBRARIES}
	${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS} simoninfo
	simonrecognitionresult simonscenariobase simonmodelmanagement )


set_target_properties( simonscenarios
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonscenarios_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenarios
  COMPONENT simondevel
)

install(FILES
${CMAKE_CURRENT_SOURCE_DIR}/simoncommandplugin.desktop DESTINATION
${SERVICETYPES_INSTALL_DIR} COMPONENT simon)
 
install( TARGETS simonscenarios DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
