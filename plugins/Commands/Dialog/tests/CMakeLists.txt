ENABLE_TESTING()
find_package(Qt4 COMPONENTS QtCore QtGui QtTest QtXml QtScript REQUIRED)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)

#################################
##### Dialog      ###############
#################################

include_directories(../)
set (simondialogcommandplugindialogtest_SRCS
  dialogtest.cpp

  #deps
  ../dialogcommandmanager.cpp 
  ../dialogconfiguration.cpp
  ../createdialogcommandwidget.cpp

  ../createtransitiondialog.cpp

  ../createtemplateoptiondialog.cpp

  ../createboundvaluedialog.cpp
 
  ../createavatardialog.cpp
  ${simondialogcommandplugintest_plasma_SRCS}
)

kde4_add_ui_files(simondialogcommandplugindialogtest_SRCS
	../dialogconfigurationdlg.ui
	../createdialogcommandwidget.ui
	../createtransitiondlg.ui
	../createtemplateoptiondialog.ui
	../createboundvalue.ui
	../createavatardlg.ui
    )

kde4_add_unit_test(simondialogcommandplugintest-dialog TESTNAME
  simondialogcommandplugin-dialog
  ${simondialogcommandplugindialogtest_SRCS}
)


target_link_libraries( simondialogcommandplugintest-dialog
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  ${simondialogcommandplugintest_plasma_LIBS}
  simonscenarios simonactions
 simondialogenginettsview simondialogenginegraphicalview
)