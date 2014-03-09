set (simondialogcommandplugin_SRCS 
  dialogcommandmanager.cpp 
  dialogconfiguration.cpp
  createdialogcommandwidget.cpp
  createtransitiondialog.cpp
)

kde4_add_ui_files(simondialogcommandplugin_SRCS 
  dialogconfigurationdlg.ui
  createdialogcommandwidget.ui
  createtransitiondlg.ui
  dialogcreateview.ui
)

kde4_add_plugin(simondialogcommandplugin ${simondialogcommandplugin_SRCS})

if (USE_PLASMA)
target_link_libraries(simondialogcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS} simonscenarios
  simonscenariobase simonactions simondialogengine simondialogenginettsview
  simondialogenginegraphicalview simondialogengineconfui)
else ()
target_link_libraries(simondialogcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simonscenarios simonscenariobase
  simonactions simondialogengine simondialogenginettsview 
  simondialogenginegraphicalview simondialogengineconfui)
endif()

install(TARGETS simondialogcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddialogplugin)
install(FILES simondialogcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddialogplugin)

add_subdirectory(tests)
