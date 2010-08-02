ENABLE_TESTING()
find_package(Qt4 COMPONENTS QtCore QtGui QtTest QtXml QtScript REQUIRED)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)

if (USE_PLASMA)
  set (simondialogcommandplugintest_plasma_SRCS
      ../plasmaboundvalue.cpp
      )
  set (simondialogcommandplugintest_plasma_LIBS
      ${KDE4_PLASMA_LIBS}
      )
endif (USE_PLASMA)



#################################
##### *BoundValue ####=##########
#################################

set (simondialogcommandpluginboundvaluetest_SRCS
  boundvaluetest.cpp

  #deps
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
)


kde4_add_unit_test(simondialogcommandplugintest-boundvalue TESTNAME
  simondialogcommandplugin-boundValue
  ${simondialogcommandpluginboundvaluetest_SRCS}
  ${simondialogcommandplugintest_plasma_SRCS}
)


target_link_libraries( simondialogcommandplugintest-boundvalue
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  ${simondialogcommandplugintest_plasma_LIBS}
)

#################################
##### DialogBoundValues #########
#################################

set (simondialogcommandplugindialogboundvaluestest_SRCS
  dialogboundvaluestest.cpp

  #deps
  ../dialogboundvalues.cpp
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
)

kde4_add_unit_test(simondialogcommandplugintest-dialogboundvalues TESTNAME
  simondialogcommandplugin-dialogboundvalues
  ${simondialogcommandplugindialogboundvaluestest_SRCS}
  ${simondialogcommandplugintest_plasma_SRCS}
)

target_link_libraries( simondialogcommandplugintest-dialogboundvalues
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  ${simondialogcommandplugintest_plasma_LIBS}
)


