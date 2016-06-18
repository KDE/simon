# This would normally come from linking to the lib, but there are cyclic dependencies...
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../simonactions)

set(simonscenarios_LIB_SRCS
  trainingmanager.cpp
  modelmanager.cpp
  wordlisttype.cpp
  vocabulary.cpp
  activevocabulary.cpp
  shadowvocabulary.cpp
  scenario.cpp
  scenariomanager.cpp
  grammar.cpp
  trainingtextcollection.cpp
  trainingtext.cpp
  languagedescriptioncontainer.cpp
  trainingcontainer.cpp
  model.cpp
  word.cpp

  author.cpp
  scenariodisplay.cpp
  scenarioobject.cpp
  commandmanager.cpp
  command.cpp
  voiceinterfacecommand.cpp
  voiceinterfacecommandtemplate.cpp
  commandconfiguration.cpp
  createcommandwidget.cpp
  createvoiceinterfacecommandwidget.cpp
  createvoiceinterfacecommandwidgetprivate.cpp
  action.cpp
  actioncollection.cpp
  actionmodel.cpp
  actioncommandmodel.cpp
  commandparameter.cpp
  promptstable.cpp
  promptstablemodel.cpp
  basemodelsettings.cpp
  createbasemodel.cpp
  modelmetadata.cpp
  versionnumber.cpp
)


set(simonscenarios_LIB_HDRS
  trainingmanager.h
  modelmanager.h
  scenario.h
  activevocabulary.h
  shadowvocabulary.h
  wordlisttype.h
  scenariomanager.h
  grammar.h
  speechmodel.h
  trainingtext.h
  word.h
  languagedescriptioncontainer.h
  trainingcontainer.h
  model.h
  author.h
  scenariodisplay.h
  scenarioobject.h
  modelmetadata.h

  commandmanager.h
  commandparameter.h
  command.h
  simoncommand.h
  commandconfiguration.h
  createcommandwidget.h
  createvoiceinterfacecommandwidget.h
  action.h
  actioncollection.h
  actionmodel.h
  actioncommandmodel.h
  commandlistelements.h
  promptstable.h
  promptstablemodel.h
  basemodelsettings.h

  scenarioofferui.h
  versionnumber.h
)


ki18n_wrap_ui(simonscenarios_LIB_SRCS createvoiceinterfacecommandwidget.ui basemodelconfig.ui modelsettingsdlg.ui)

qt5_add_dbus_adaptor(simonscenarios_LIB_SRCS org.simon-listens.ScenarioManager.xml scenariomanager.h ScenarioManager)

kconfig_add_kcfg_files(simonscenarios_LIB_SRCS speechmodelmanagementconfiguration.kcfgc)

kde4_add_library(simonscenarios SHARED ${simonscenarios_LIB_SRCS})
target_link_libraries(simonscenarios ${QT_LIBRARIES}
  KF5::KDELibs4Support KF5::KIOCore KF5::Service KF5::NewStuff
  KF5::WidgetsAddons KF5::CoreAddons KF5::SonnetCore KF5::SonnetUi
  simonxml simonlogging simoninfo simonrecognitionresult
  simoncontextdetection simongraphemetophoneme)

generate_export_header(simonscenarios)

set_target_properties(simonscenarios
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
  DEFINE_SYMBOL MAKE_MODELMANAGEMENT_LIB
)

install(FILES ${simonscenarios_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenarios
  COMPONENT simondevel
)
 
install(TARGETS simonscenarios DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
install(FILES speechmodelmanagementconfiguration.kcfg DESTINATION ${KCFG_INSTALL_DIR} COMPONENT simoncore)

install(FILES simoncommandplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR} COMPONENT simoncore)
 
##########
set(kcm_simonscenarios_LIB_SRCS
  speechmodelsettings.cpp
)
ki18n_wrap_ui(kcm_simonscenarios_LIB_SRCS trainingsettings.ui languageprofilesettings.ui)
kconfig_add_kcfg_files(kcm_simonscenarios_LIB_SRCS speechmodelmanagementconfiguration.kcfgc)

kde4_add_library(simonscenarioskcm MODULE ${kcm_simonscenarios_LIB_SRCS})
target_link_libraries(simonscenarioskcm  KF5::KIOCore KF5::WidgetsAddons
  KF5::CoreAddons KF5::KDELibs4Support KF5::SonnetCore KF5::SonnetUi
  simonscenarios KF5::NewStuff KF5::Service Qt5::Core)

install(TARGETS simonscenarioskcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore)

install(FILES simonspeechmodelmanagementconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore)
install(FILES simonbasemodels.knsrc DESTINATION ${CONFIG_INSTALL_DIR} COMPONENT simon)
