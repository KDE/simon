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

add_executable(simondialogcommandplugintest-boundvalueNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-boundValue
  ${simondialogcommandpluginboundvaluetest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-boundvalue
  KF5::KDELibs4Support Qt5::Test Qt5::Gui
  Qt5::Xml ${simondialogcommandplugintest_plasma_LIBS}
  Qt5::Script ${PLAMSA_DEP_LIB}
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

add_executable(simondialogcommandplugintest-dialogboundvaluesNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-dialogboundvalues
  ${simondialogcommandplugindialogboundvaluestest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogboundvalues
  KF5::KDELibs4Support Qt5::Test Qt5::Gui
  Qt5::Xml ${simondialogcommandplugintest_plasma_LIBS}
  Qt5::Script ${PLAMSA_DEP_LIB}
)

#################################
##### DialogTemplateOptions #####
#################################

set(simondialogcommandplugindialogtemplateoptionstest_SRCS
  dialogtemplateoptionstest.cpp

  #deps
  ../dialogtemplateoptions.cpp
)

add_executable(simondialogcommandplugintest-dialogtemplateoptionsNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-dialogtemplateoptions
  ${simondialogcommandplugindialogtemplateoptionstest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogtemplateoptions
  KF5::KDELibs4Support Qt5::Test Qt5::Gui
  Qt5::Xml
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

add_executable(simondialogcommandplugintest-dialogtextparserNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-dialogtextparser
  ${simondialogcommandplugindialogtextparsertest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogtextparser
  KF5::KDELibs4Support Qt5::Test Qt5::Gui
  Qt5::Xml ${simondialogcommandplugintest_plasma_LIBS}
  Qt5::Script ${PLAMSA_DEP_LIB}
)

#################################
##### DialogCommand #############
#################################

set(simondialogcommandplugindialogcommandtest_SRCS
  dialogcommandtest.cpp

  #deps
  ../dialogcommand.cpp
)

add_executable(simondialogcommandplugintest-dialogcommandNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-dialogcommand
  ${simondialogcommandplugindialogcommandtest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogcommand
  KF5::KDELibs4Support Qt5::Test Qt5::Gui ${simondialogcommandplugintest_plasma_LIBS}
  Qt5::Xml  simonscenarios simonactions
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

add_executable(simondialogcommandplugintest-dialogstateNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-dialogstate
  ${simondialogcommandplugindialogstatetest_SRCS}
)

target_link_libraries(simondialogcommandplugintest-dialogstate
  KF5::KDELibs4Support Qt5::Test Qt5::Gui
  ${simondialogcommandplugintest_plasma_LIBS}
  Qt5::Xml simonscenarios simonactions
  Qt5::Script ${PLAMSA_DEP_LIB}
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

add_executable(simondialogcommandplugintest-avatarsNAME
add_test(simondialogcommandplugintest simondialogcommandplugintest)
ecm_mark_as_test(simondialogcommandplugintest)
  simondialogcommandplugin-avatars
  ${simonavatarstest_SRCS}
)


target_link_libraries(simondialogcommandplugintest-avatars
  KF5::KDELibs4Support Qt5::Test Qt5::Gui
  Qt5::Xml
)
