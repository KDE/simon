set (simonactivewindowplugin_SRCS 
  activewindow.cpp 
  createactivewindowwidget.cpp
)

kde4_add_ui_files(simonactivewindowplugin_SRCS createactivewindowwidget.ui)

kde4_add_plugin(simonactivewindowplugin ${simonactivewindowplugin_SRCS})

target_link_libraries(simonactivewindowplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simoncontextdetection simonuicomponents)

install(TARGETS simonactivewindowplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonactivewindowplugin)
install(FILES simonactivewindowplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simonactivewindowplugin)
