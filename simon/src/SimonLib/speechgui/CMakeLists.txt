include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( speechgui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidgetview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonlineedit.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simontablewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonspinbox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simongroupbox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonslider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simoncombobox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simoncalendarwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/guievents.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonmainwindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/returntablewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/inlinewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonlistwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simontabwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/addserverconnection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/serveraddressselector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trayiconmanager.cpp
)

set( speechgui_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/speechgui_export.h
	${CMAKE_CURRENT_SOURCE_DIR}/eventhandler.h
)


kde4_add_library( speechgui  SHARED ${speechgui_LIB_SRCS} )

target_link_libraries( speechgui ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} )


set_target_properties( speechgui
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${speechgui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechgui
  COMPONENT Devel
)
 
install( TARGETS speechgui  ${INSTALL_TARGETS_DEFAULT_ARGS} )