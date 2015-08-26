set (simondbusconditionplugin_SRCS 
  dbuscondition.cpp 
  createdbusconditionwidget.cpp
)

ki18n_wrap_ui(simondbusconditionplugin_SRCS createdbusconditionwidget.ui)

kde4_add_plugin(simondbusconditionplugin ${simondbusconditionplugin_SRCS})

target_link_libraries(simondbusconditionplugin  KF5::KIOCore
  Qt5::Xml simoncontextdetection)

install(TARGETS simondbusconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simondbusconditionplugin)
install(FILES simondbusconditionplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simondbusconditionplugin)
