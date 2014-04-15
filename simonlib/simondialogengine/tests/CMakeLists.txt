if (USE_PLASMA)
  set(PLAMSA_DEP_TEST_SRC ../plasmaboundvalue.cpp)
else()
  set(PLAMSA_DEP_TEST_SRC "")
endif()

#################################
##### BoundValue ################
#################################

set(simondialogcommandpluginboundvaluetest_SRCS
  boundvaluetest.cpp

  #deps
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
  ../argumentboundvalue.cpp
  ${PLAMSA_DEP_TEST_SRC}
)

kde4_add_unit_test(simondialogcommandplugintest-boundvalue TESTNAME
  simondialogcommandplugin-boundValue
  ${simondialogcommandpluginboundvaluetest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-boundvalue
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY} ${simondialogcommandplugintest_plasma_LIBS}
  ${QT_QTSCRIPT_LIBRARY} ${PLAMSA_DEP_LIB}
)

#################################
##### DialogBoundValues #########
#################################

set(simondialogcommandplugindialogboundvaluestest_SRCS
  dialogboundvaluestest.cpp

  #deps
  ../dialogboundvalues.cpp
  ../boundvalue.cpp
  ../staticboundvalue.cpp
  ../scriptboundvalue.cpp
  ../argumentboundvalue.cpp
  ${PLAMSA_DEP_TEST_SRC}
)

kde4_add_unit_test(simondialogcommandplugintest-dialogboundvalues TESTNAME
  simondialogcommandplugin-dialogboundvalues
  ${simondialogcommandplugindialogboundvaluestest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogboundvalues
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY} ${simondialogcommandplugintest_plasma_LIBS}
  ${QT_QTSCRIPT_LIBRARY} ${PLAMSA_DEP_LIB}
)

#################################
##### DialogTemplateOptions #####
#################################

set(simondialogcommandplugindialogtemplateoptionstest_SRCS
  dialogtemplateoptionstest.cpp

  #deps
  ../dialogtemplateoptions.cpp
)

kde4_add_unit_test(simondialogcommandplugintest-dialogtemplateoptions TESTNAME
  simondialogcommandplugin-dialogtemplateoptions
  ${simondialogcommandplugindialogtemplateoptionstest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogtemplateoptions
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
)

#################################
##### DialogTextParser ##########
#################################

set(simondialogcommandplugindialogtextparsertest_SRCS
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
  ${PLAMSA_DEP_TEST_SRC}
)

kde4_add_unit_test(simondialogcommandplugintest-dialogtextparser TESTNAME
  simondialogcommandplugin-dialogtextparser
  ${simondialogcommandplugindialogtextparsertest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogtextparser
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY} ${simondialogcommandplugintest_plasma_LIBS}
  ${QT_QTSCRIPT_LIBRARY} ${PLAMSA_DEP_LIB}
)

#################################
##### DialogCommand #############
#################################

set(simondialogcommandplugindialogcommandtest_SRCS
  dialogcommandtest.cpp

  #deps
  ../dialogcommand.cpp
)

kde4_add_unit_test(simondialogcommandplugintest-dialogcommand TESTNAME
  simondialogcommandplugin-dialogcommand
  ${simondialogcommandplugindialogcommandtest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogcommand
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY} ${simondialogcommandplugintest_plasma_LIBS}
  ${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS} simonscenarios simonactions
)

#################################
##### DialogState ###############
#################################

set(simondialogcommandplugindialogstatetest_SRCS
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
  ${PLAMSA_DEP_TEST_SRC}
)

kde4_add_unit_test(simondialogcommandplugintest-dialogstate TESTNAME
  simondialogcommandplugin-dialogstate
  ${simondialogcommandplugindialogstatetest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogstate
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${simondialogcommandplugintest_plasma_LIBS}
  ${QT_QTXML_LIBRARY} simonscenarios simonactions
  ${QT_QTSCRIPT_LIBRARY} ${PLAMSA_DEP_LIB} ${KDE4_KDEUI_LIBS}
)


#################################
##### Avatars ###################
#################################

set(simonavatarstest_SRCS
  dialogavatartest.cpp

  #deps
  ../avatar.cpp
  ../avatarmodel.cpp
)

#enable exports for the test
add_definitions(-DMAKE_SIMONDIALOGENGINE_LIB)

kde4_add_unit_test(simondialogcommandplugintest-avatars TESTNAME
  simondialogcommandplugin-avatars
  ${simonavatarstest_SRCS}
)


target_link_libraries(simondialogcommandplugintest-avatars
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
)


#################################
##### Dialog Variables ##########
#################################

set(simonvariabletest_SRCS
  dialogvariabletest.cpp

  #deps
  ../dialogvariablestore.cpp
)

#enable exports for the test
#add_definitions(-DMAKE_SIMONDIALOGENGINE_LIB)

kde4_add_unit_test(simondialogcommandplugintest-variables TESTNAME
  simondialogcommandplugin-variables
  ${simonvariabletest_SRCS}
)


target_link_libraries(simondialogcommandplugintest-variables
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
)
