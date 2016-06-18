set (simondialogcommandplugin_SRCS 
  dialogcommandmanager.cpp 
  dialogconfiguration.cpp
  createdialogcommandwidget.cpp
  createtransitiondialog.cpp
)

ki18n_wrap_ui(simondialogcommandplugin_SRCS
  dialogconfigurationdlg.ui
  createdialogcommandwidget.ui
  createtransitiondlg.ui
)

kde4_add_library(simondialogcommandplugin ${simondialogcommandplugin_SRCS})

if (USE_PLASMA)
target_link_libraries(simondialogcommandplugin ${KDE4_PLASMA_LIBS})
endif ()

target_link_libraries(simondialogcommandplugin
  ${QT_LIBRARIES} KF5::KIOCore simonscenarios
  simonactions simondialogengine simondialogenginettsview
  simondialogenginegraphicalview simondialogengineconfui eventsimulation)

install(TARGETS simondialogcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddialogplugin)
install(FILES simondialogcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddialogplugin)

# add_subdirectory(tests)
