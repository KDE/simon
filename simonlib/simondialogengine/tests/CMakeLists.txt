ENABLE_TESTING()
find_package(Qt4 COMPONENTS QtCore QtGui QtTest QtXml QtScript REQUIRED)


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
##### BoundValue ################
#################################

set (simondialogcommandpluginboundvaluetest_SRCS
  boundvaluetest.cpp

  #deps
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
  ../argumentboundvalue.cpp
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
  ../argumentboundvalue.cpp
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

#################################
##### DialogTemplateOptions #####
#################################

set (simondialogcommandplugindialogtemplateoptionstest_SRCS
  dialogtemplateoptionstest.cpp

  #deps
  ../dialogtemplateoptions.cpp
)

kde4_add_unit_test(simondialogcommandplugintest-dialogtemplateoptions TESTNAME
  simondialogcommandplugin-dialogtemplateoptions
  ${simondialogcommandplugindialogtemplateoptionstest_SRCS}
)

target_link_libraries( simondialogcommandplugintest-dialogtemplateoptions
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
)

#################################
##### DialogTextParser ##########
#################################

set (simondialogcommandplugindialogtextparsertest_SRCS
  dialogtextparsertest.cpp

  #deps
  ../dialogtextparser.cpp
  ../dialogtext.cpp
  ../dialogtemplateoptions.cpp
  ../dialogboundvalues.cpp
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
  ../argumentboundvalue.cpp
)

kde4_add_unit_test(simondialogcommandplugintest-dialogtextparser TESTNAME
  simondialogcommandplugin-dialogtextparser
  ${simondialogcommandplugindialogtextparsertest_SRCS}
  ${simondialogcommandplugintest_plasma_SRCS}
)

target_link_libraries( simondialogcommandplugintest-dialogtextparser
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  ${simondialogcommandplugintest_plasma_LIBS}
)

#################################
##### DialogCommand #############
#################################

set (simondialogcommandplugindialogcommandtest_SRCS
  dialogcommandtest.cpp

  #deps
  ../dialogcommand.cpp
)

kde4_add_unit_test(simondialogcommandplugintest-dialogcommand TESTNAME
  simondialogcommandplugin-dialogcommand
  ${simondialogcommandplugindialogcommandtest_SRCS}
)

target_link_libraries( simondialogcommandplugintest-dialogcommand
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  simonscenarios simonactions
)

#################################
##### DialogState ###############
#################################

set (simondialogcommandplugindialogstatetest_SRCS
  dialogstatetest.cpp

  #deps
  ../dialogstate.cpp
  ../dialogcommand.cpp

  ../dialogtextparser.cpp
  ../dialogtext.cpp
  ../dialogtemplateoptions.cpp
  ../dialogboundvalues.cpp
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
  ../argumentboundvalue.cpp
  ${simondialogcommandplugintest_plasma_SRCS}
)

kde4_add_unit_test(simondialogcommandplugintest-dialogstate TESTNAME
  simondialogcommandplugin-dialogstate
  ${simondialogcommandplugindialogstatetest_SRCS}
)

target_link_libraries( simondialogcommandplugintest-dialogstate
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  ${simondialogcommandplugintest_plasma_LIBS}
  simonscenarios simonactions
)


#################################
##### Avatars ###################
#################################

set (simonavatarstest_SRCS
  dialogavatartest.cpp

  #deps
  ../avatar.cpp
  ../avatarmodel.cpp
)


kde4_add_unit_test(simondialogcommandplugintest-avatars TESTNAME
  simondialogcommandplugin-avatars
  ${simonavatarstest_SRCS}
)


target_link_libraries( simondialogcommandplugintest-avatars
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
)
