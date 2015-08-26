set (simonactivewindowplugin_SRCS 
  activewindow.cpp 
  createactivewindowwidget.cpp
)

ki18n_wrap_ui(simonactivewindowplugin_SRCS createactivewindowwidget.ui)

kde4_add_plugin(simonactivewindowplugin ${simonactivewindowplugin_SRCS})

target_link_libraries(simonactivewindowplugin
  Qt5::Xml simoncontextdetection simonuicomponents)

install(TARGETS simonactivewindowplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonactivewindowplugin)
install(FILES simonactivewindowplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simonactivewindowplugin)
