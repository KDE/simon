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
)


set(simonscenarios_LIB_HDRS
  simonmodelmanagement_export.h
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
)



kde4_add_ui_files(simonscenarios_LIB_SRCS createvoiceinterfacecommandwidget.ui basemodelconfig.ui modelsettingsdlg.ui)

qt4_add_dbus_adaptor(simonscenarios_LIB_SRCS org.simon-listens.ScenarioManager.xml scenariomanager.h ScenarioManager)

kde4_add_kcfg_files(simonscenarios_LIB_SRCS speechmodelmanagementconfiguration.kcfgc)

kde4_add_library(simonscenarios SHARED ${simonscenarios_LIB_SRCS})
target_link_libraries(simonscenarios ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS}
  simonxml simonlogging simoninfo simonrecognitionresult ${KDE4_KNEWSTUFF3_LIBS}
  simonscenariobase simoncontextdetection simongraphemetophoneme)

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
kde4_add_ui_files(kcm_simonscenarios_LIB_SRCS trainingsettings.ui languageprofilesettings.ui)
kde4_add_kcfg_files(kcm_simonscenarios_LIB_SRCS speechmodelmanagementconfiguration.kcfgc)

kde4_add_plugin(simonscenarioskcm ${kcm_simonscenarios_LIB_SRCS})
target_link_libraries(simonscenarioskcm ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  simonscenarios simonscenariobase ${KDE4_KNEWSTUFF3_LIBS})

install(TARGETS simonscenarioskcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore)

install(FILES simonspeechmodelmanagementconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore)
install(FILES simonbasemodels.knsrc DESTINATION ${CONFIG_INSTALL_DIR} COMPONENT simon)
