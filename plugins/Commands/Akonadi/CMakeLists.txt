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

ki18n_wrap_ui(simonakonadicommandplugin_SRCS
  akonadiconfigurationdlg.ui
  createakonadicommandwidget.ui
  timeselector.ui
)

kde4_add_library(simonakonadicommandplugin MODULE ${simonakonadicommandplugin_SRCS})

target_link_libraries(simonakonadicommandplugin  Qt5::Xml
  KF5::AkonadiCore KF5::CalendarCore simonscenariobase simonscenarios simonlogging simonactions
  simondialogengine simondialogengineconfui simondialogenginegraphicalview simondialogenginettsview)

install(TARGETS simonakonadicommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandakonadiplugin)
install(FILES simonakonadicommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandakonadiplugin)
