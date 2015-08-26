include_directories(../)
set (simondialogcommandplugindialogtest_SRCS
  dialogtest.cpp

  #deps
  ../dialogcommandmanager.cpp
  ../dialogconfiguration.cpp
  ../createtransitiondialog.cpp
  ../createdialogcommandwidget.cpp
)

ki18n_wrap_ui(simondialogcommandplugindialogtest_SRCS
  ../dialogconfigurationdlg.ui
  ../createdialogcommandwidget.ui
  ../createtransitiondlg.ui
)

add_test(simondialogcommandplugintest-dialog NAME
  simondialogcommandplugin-dialog
  ${simondialogcommandplugindialogtest_SRCS}
)


target_link_libraries( simondialogcommandplugintest-dialog
  KF5::KDELibs4Support KF5::KIOCore Qt5::Test ${QT_LIBRARIES}
  ${simondialogcommandplugintest_plasma_LIBS}
  simonscenarios simonactions simondialogengine
  simondialogenginettsview simondialogenginegraphicalview
  simondialogengineconfui
)
