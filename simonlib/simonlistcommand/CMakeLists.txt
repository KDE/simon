
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonlistcommand_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/listcommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandlistwidget.cpp
	#	${CMAKE_CURRENT_SOURCE_DIR}/createlistcommandwidget.cpp
)

set( simonlistcommand_LIB_HDRS
  listcommand.h
  commandlistwidget.h
  #  createlistcommandwidget.h
)

kde4_add_ui_files(simonlistcommand_LIB_SRCS 
	#createlistcommandwidget.ui
	listcommand.ui
)

kde4_add_library( simonlistcommand  SHARED
	${simonlistcommand_LIB_SRCS} )

target_link_libraries( simonlistcommand ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
	simonactions simonscenarios )


set_target_properties( simonlistcommand
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonlistcommand_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/listcommand
  COMPONENT simondevel
)

install( TARGETS simonlistcommand DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
