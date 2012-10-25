set (simonakonadicommandplugin_SRCS 
  akonadicommandmanager.cpp 
  akonadiconfiguration.cpp
  akonadicommand.cpp
  timeselector.cpp
  createakonadicommandwidget.cpp
  scheduleitem.cpp
  alarmscheduleitem.cpp
  commandscheduleitem.cpp
  dialogrunner.cpp
)

kde4_add_ui_files(simonakonadicommandplugin_SRCS 
  akonadiconfigurationdlg.ui
  createakonadicommandwidget.ui
  timeselector.ui
)

kde4_add_plugin(simonakonadicommandplugin ${simonakonadicommandplugin_SRCS})

target_link_libraries(simonakonadicommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  ${KDEPIMLIBS_AKONADI_LIBS} ${KDEPIMLIBS_KCALCORE_LIBS} simonscenariobase simonscenarios simonlogging simonactions
  simondialogengine simondialogengineconfui simondialogenginegraphicalview simondialogenginettsview)

install(TARGETS simonakonadicommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandakonadiplugin)
install(FILES simonakonadicommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandakonadiplugin)
