set (simondbusconditionplugin_SRCS 
  dbuscondition.cpp 
  createdbusconditionwidget.cpp
)

kde4_add_ui_files(simondbusconditionplugin_SRCS createdbusconditionwidget.ui)

kde4_add_plugin(simondbusconditionplugin ${simondbusconditionplugin_SRCS})

target_link_libraries(simondbusconditionplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  ${QT_QTXML_LIBRARY} simoncontextdetection)

install(TARGETS simondbusconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simondbusconditionplugin)
install(FILES simondbusconditionplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simondbusconditionplugin)
