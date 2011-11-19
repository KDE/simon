find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${ATSPI_INCLUDE_DIR} ${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} 
${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)


FIND_LIBRARY(X11_X11_LIB X11 ${X11_LIB_SEARCH_PATH})

set (simonatspicommandplugin_SRCS 
	atspicommandmanager.cpp 
	atspiconfiguration.cpp
	dbusconnection.cpp
        atspiaction.cpp
        accessibleobject.cpp
        qt-atspi.cpp
        #atspiwatcher.cpp
    )

kde4_add_ui_files(simonatspicommandplugin_SRCS 
	atspiconfigurationdlg.ui
    )

kde4_add_plugin(simonatspicommandplugin ${simonatspicommandplugin_SRCS})


target_link_libraries(simonatspicommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
	simonscenarios ${X11_X11_LIB} ${ATSPI_LIBRARY} )

install(TARGETS simonatspicommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandatspiplugin )
install( FILES simonatspicommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandatspiplugin )
add_subdirectory(po)
