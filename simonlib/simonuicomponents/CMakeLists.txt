
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonuicomponents_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidgetview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/addserverconnection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/serveraddressselector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/selectionlistview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trayiconmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/textlistwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonwizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/multikcmview.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/selectprogramdialog.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/actionbutton.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/verticalscrollarea.cpp

)

set( simonuicomponents_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/simonuicomponents_export.h
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidgetview.h
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/addserverconnection.h
	${CMAKE_CURRENT_SOURCE_DIR}/selectionlistview.h
	${CMAKE_CURRENT_SOURCE_DIR}/serveraddressselector.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonwizard.h
	${CMAKE_CURRENT_SOURCE_DIR}/trayiconmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/multikcmview.h
	${CMAKE_CURRENT_SOURCE_DIR}/textlistwidget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/selectprogramdialog.h
        ${CMAKE_CURRENT_SOURCE_DIR}/actionbutton.h
        ${CMAKE_CURRENT_SOURCE_DIR}/verticalscrollarea.h
)

kde4_add_ui_files(simonuicomponents_LIB_SRCS
        selectprogramdlg.ui
    )

kde4_add_library( simonuicomponents  SHARED ${simonuicomponents_LIB_SRCS} )

target_link_libraries( simonuicomponents ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )


set_target_properties( simonuicomponents
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonuicomponents_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonuicomponents
  COMPONENT simoncoredevel
)
 
install( TARGETS simonuicomponents DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
