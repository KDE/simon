include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonactions_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandtablemodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/actionmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/greedyreceiver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/listcommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandlistwidget.cpp
)

set( simonactions_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/commandmodel.h
	${CMAKE_CURRENT_SOURCE_DIR}/actionmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/greedyreceiver.h
  	${CMAKE_CURRENT_SOURCE_DIR}/listcommand.h
	${CMAKE_CURRENT_SOURCE_DIR}/commandlistwidget.h
)

kde4_add_ui_files( simonactions_LIB_SRCS
		${CMAKE_CURRENT_SOURCE_DIR}/commandsettingsdlg.ui
	)

kde4_add_library( simonactions  SHARED ${simonactions_LIB_SRCS} )

target_link_libraries( simonactions ${QT_LIBRARIES} ${QT_QTXML_LIBRARY}
${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}  simonscenarios simonrecognitionresult
simonscenariobase)

set_target_properties( simonactions
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonactions_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonactions
  COMPONENT simondevel
)
 
install( TARGETS simonactions DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
install( FILES simonactionsconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
