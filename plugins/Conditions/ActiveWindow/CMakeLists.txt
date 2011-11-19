
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonactivewindowplugin_SRCS 
	activewindow.cpp 
        createactivewindowwidget.cpp
    )

kde4_add_ui_files(simonactivewindowplugin_SRCS
        createactivewindowwidget.ui
    )

kde4_add_plugin(simonactivewindowplugin ${simonactivewindowplugin_SRCS})


target_link_libraries(simonactivewindowplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simoncontextdetection simonuicomponents)

install(TARGETS simonactivewindowplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonactivewindowplugin )
install( FILES simonactivewindowplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonactivewindowplugin )
