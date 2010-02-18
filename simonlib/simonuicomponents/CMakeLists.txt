include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonuicomponents_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidgetview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/addserverconnection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/serveraddressselector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trayiconmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/textlistwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/multikcmview.cpp

	#${CMAKE_CURRENT_SOURCE_DIR}/simonmainwindow.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simonlineedit.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simontablewidget.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simonspinbox.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simongroupbox.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simonslider.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simonwidget.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simoncombobox.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simoncalendarwidget.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/guievents.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/returntablewidget.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simonlistwidget.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/simontabwidget.cpp

)

set( simonuicomponents_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/simonuicomponents_export.h
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidgetview.h
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/addserverconnection.h
	${CMAKE_CURRENT_SOURCE_DIR}/serveraddressselector.h
	${CMAKE_CURRENT_SOURCE_DIR}/trayiconmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/multikcmview.h
	${CMAKE_CURRENT_SOURCE_DIR}/textlistwidget.h

	#${CMAKE_CURRENT_SOURCE_DIR}/simonlineedit.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simontablewidget.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simonspinbox.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simongroupbox.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simonslider.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simonwidget.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simoncombobox.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simoncalendarwidget.h
	#${CMAKE_CURRENT_SOURCE_DIR}/guievents.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simonmainwindow.h
	#${CMAKE_CURRENT_SOURCE_DIR}/returntablewidget.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simonlistwidget.h
	#${CMAKE_CURRENT_SOURCE_DIR}/simontabwidget.h
)

kde4_add_library( simonuicomponents  SHARED ${simonuicomponents_LIB_SRCS} )

target_link_libraries( simonuicomponents ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )


set_target_properties( simonuicomponents
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonuicomponents_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonuicomponents
  COMPONENT simoncoredevel
)
 
install( TARGETS simonuicomponents DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
