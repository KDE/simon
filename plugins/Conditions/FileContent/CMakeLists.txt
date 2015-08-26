set (simonfilecontentplugin_SRCS 
  filecontent.cpp 
  createfilecontentwidget.cpp
)

ki18n_wrap_ui(simonfilecontentplugin_SRCS createfilecontentwidget.ui)

kde4_add_plugin(simonfilecontentplugin ${simonfilecontentplugin_SRCS})

target_link_libraries(simonfilecontentplugin  KF5::KIOCore
  Qt5::Xml simoncontextdetection)

install(TARGETS simonfilecontentplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonfilecontentplugin)
install(FILES simonfilecontentplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simonfilecontentplugin)
