include(FindQt4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonactionsui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandpreviewwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/newcommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/manageactionsdialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/manageactionsautorunwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/runcommandview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/runcommandviewprivate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/addactiondialog.cpp
)

set( simonactionsui_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/runcommandview.h
)

kde4_add_ui_files( simonactionsui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/commandpreviewwidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/modifycommands.ui
	${CMAKE_CURRENT_SOURCE_DIR}/rundialog.ui
	${CMAKE_CURRENT_SOURCE_DIR}/manageactionsdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/manageactionsautorunwidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/addactiondlg.ui
)

kde4_add_library( simonactionsui  SHARED ${simonactionsui_LIB_SRCS} )

target_link_libraries( simonactionsui ${QT_LIBRARIES}
${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} simonactions
simonscenarios simonscenarios simonuicomponents)


set_target_properties( simonactionsui
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonactionsui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonactionsui
  COMPONENT simondevel
)
 
install( TARGETS simonactionsui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
