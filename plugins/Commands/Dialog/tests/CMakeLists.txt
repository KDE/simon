include_directories(../)
set (simondialogcommandplugindialogtest_SRCS
  dialogtest.cpp

  #deps
  ../dialogcommandmanager.cpp
  ../dialogconfiguration.cpp
  ../createtransitiondialog.cpp
  ../createdialogcommandwidget.cpp
)

kde4_add_ui_files(simondialogcommandplugindialogtest_SRCS
  ../dialogconfigurationdlg.ui
  ../createdialogcommandwidget.ui
  ../createtransitiondlg.ui
)

kde4_add_unit_test(simondialogcommandplugintest-dialog TESTNAME
  simondialogcommandplugin-dialog
  ${simondialogcommandplugindialogtest_SRCS}
)


target_link_libraries( simondialogcommandplugintest-dialog
  ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES}
  ${simondialogcommandplugintest_plasma_LIBS}
  simonscenarios simonactions simondialogengine
  simondialogenginettsview simondialogenginegraphicalview
  simondialogengineconfui
)
