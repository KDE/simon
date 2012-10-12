set (simonfilecontentplugin_SRCS 
  filecontent.cpp 
  createfilecontentwidget.cpp
)

kde4_add_ui_files(simonfilecontentplugin_SRCS createfilecontentwidget.ui)

kde4_add_plugin(simonfilecontentplugin ${simonfilecontentplugin_SRCS})

target_link_libraries(simonfilecontentplugin ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  ${QT_QTXML_LIBRARY} simoncontextdetection)

install(TARGETS simonfilecontentplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonfilecontentplugin)
install(FILES simonfilecontentplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simonfilecontentplugin)
