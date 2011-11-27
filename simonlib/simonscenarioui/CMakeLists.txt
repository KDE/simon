
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonscenarioui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/scenariomanagementdialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/newscenario.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/newauthor.cpp
)

set( simonscenarioui_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/scenariomanagementdialog.h
)

kde4_add_ui_files( simonscenarioui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modifyscenario.ui
	${CMAKE_CURRENT_SOURCE_DIR}/modifyauthor.ui
	${CMAKE_CURRENT_SOURCE_DIR}/scenariomanagementdlg.ui
)

kde4_add_library( simonscenarioui  SHARED ${simonscenarioui_LIB_SRCS} )


target_link_libraries( simonscenarioui ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} 
	simonscenarios simonscenariobase ${KDE4_KIO_LIBS} ${KDE4_KNEWSTUFF3_LIBS})


set_target_properties( simonscenarioui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonscenarioui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenarioui
  COMPONENT simondevel
)
 
install( TARGETS simonscenarioui DESTINATION ${SIMON_LIB_INSTALL_DIR}
	COMPONENT simoncore )
