set(simonmodelmanagementui_LIB_SRCS
  deleteworddialog.cpp
  editworddialog.cpp
  dragtableview.cpp
  droplistwidget.cpp
  trainingview.cpp
  trainingviewprivate.cpp
  grammarview.cpp
  grammarviewprivate.cpp
  promptsview.cpp
  promptsviewprivate.cpp
  editsampledialog.cpp

  ImportTrainingTexts/xmltrainingtextlist.cpp
  ImportTrainingTexts/importtrainingtextselectsourcepage.cpp
  ImportTrainingTexts/importtrainingtextlocalpage.cpp
  ImportTrainingTexts/importtrainingtexts.cpp
  ImportTrainingTexts/importtrainingtextworkingpage.cpp
  ImportTrainingTexts/importtrainingtextaddpage.cpp
  ImportTrainingData/importtrainingdirectory.cpp
  ImportTrainingData/importtrainingdirectoryworkingpage.cpp
  ImportTrainingData/importtrainingdata.cpp
  ImportTrainingData/importtrainingdirectoryintropage.cpp
  AddWord/addwordview.cpp
  AddWord/addwordresolvepage.cpp
  AddWord/addwordintropage.cpp
  AddWord/addwordrecordpage.cpp
  ImportDict/importdictselectsourcepage.cpp
  ImportDict/importlexiconpage.cpp
  ImportDict/importdict.cpp
  ImportDict/importdictintropage.cpp
  ImportDict/importdictworkingpage.cpp
  ImportDict/bompdict.cpp
  ImportDict/sphinxdict.cpp
  ImportDict/importdictview.cpp
  ImportDict/importdictbomppage.cpp
  ImportDict/importdictbompdownloadpage.cpp
  ImportDict/importdictsphinxpage.cpp
  ImportDict/importdictjuliuspage.cpp
  ImportDict/importdictplspage.cpp
  ImportDict/lexicondict.cpp
  ImportDict/plsdict.cpp
  ImportDict/juliusvocabulary.cpp
  ImportDict/dict.cpp
  MergeCategories/mergecategories.cpp
  MergeCategories/mergecategoriesworkingpage.cpp
  MergeCategories/mergecategoriesselectcategoriespage.cpp
  MergeCategories/mergecategorieswizard.cpp
  ImportGrammar/importgrammarwizard.cpp
  ImportGrammar/importgrammarworkingpage.cpp
  ImportGrammar/importgrammarselectinputpage.cpp
  ImportGrammar/importgrammar.cpp
  RenameCategory/renamecategorywizard.cpp
  RenameCategory/renamecategoryworkingpage.cpp
  RenameCategory/renamecategoryselectparameterspage.cpp
  RenameCategory/renamecategory.cpp
  TrainSamples/trainingswizard.cpp
  TrainSamples/trainsampleintropage.cpp
  TrainSamples/trainsamplepage.cpp
  CreateLanguageProfile/languageprofileview.cpp
)

ki18n_wrap_ui(simonmodelmanagementui_LIB_SRCS
  modelextensionsettingsdlg.ui
  removeword.ui
  editword.ui
  grammarview.ui
  trainmain.ui
  prompts.ui
  editsample.ui
  ImportTrainingTexts/importtrainingtextlocalpage.ui
  ImportTrainingTexts/importtrainingtextworkingpage.ui
  ImportTrainingTexts/importtrainingtextaddpage.ui
  ImportTrainingTexts/importtrainingtextselectsourcepage.ui
  ImportTrainingData/importtrainingdataintrodlg.ui
  ImportTrainingData/importtrainingdataworkingdlg.ui
  AddWord/addwordintropage.ui
  AddWord/resolvewordpage.ui
  ImportDict/importdictbomppage.ui
  ImportDict/importdictbompdownloadpage.ui
  ImportDict/importdictsphinxpage.ui
  ImportDict/importdictjuliuspage.ui
  ImportDict/importdictplspage.ui
  ImportDict/importdictlexiconpage.ui
  ImportDict/importdictselectsourcepage.ui
  MergeCategories/mergecategoriesworkingpage.ui
  MergeCategories/mergecategoriesselectpage.ui
  ImportGrammar/selectinputpage.ui
  ImportGrammar/importgrammarworkingpage.ui
  RenameCategory/renamecategoryselectparameterspage.ui
  RenameCategory/renamecategoryworkingpage.ui
  CreateLanguageProfile/languageprofileview.ui
)

set(simonmodelmanagementui_LIB_HDRS
  trainingview.h
  grammarview.h
  vocabularyview.h
  promptsview.h
  simonmodelmanagementui_export.h
)

set( simonmodelmanagementui_LIB_SRCS
  ${simonmodelmanagementui_LIB_SRCS}
  vocabularyview.cpp
  vocabularyviewprivate.cpp
)

ki18n_wrap_ui( simonmodelmanagementui_LIB_SRCS vocabulary.ui)
kconfig_add_kcfg_files(simonmodelmanagementui_LIB_SRCS speechmodelmanagementuiconfiguration.kcfgc)

kde4_add_library(simonmodelmanagementui SHARED ${simonmodelmanagementui_LIB_SRCS})

target_link_libraries( simonmodelmanagementui ${QT_LIBRARIES} 
  KF5::KIOCore simonlogging simonscenarios simonscenariobase simoninfo simonxml
  simonsound simonuicomponents simongraphemetophoneme KF5::Codecs
)

set_target_properties( simonmodelmanagementui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR} DEFINE_SYMBOL MAKE_SIMONMODELMANAGEMENTUI_LIB
)

install( FILES ${simonmodelmanagementui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonmodelmanagementui
  COMPONENT simondevel
)

install( TARGETS simonmodelmanagementui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )

set(kcm_simonmodelmanagementui_LIB_SRCS 
  modelextensionsettings.cpp
  plugin_exporter.cpp
)

ki18n_wrap_ui( kcm_simonmodelmanagementui_LIB_SRCS modelextensionsettingsdlg.ui)

kconfig_add_kcfg_files(kcm_simonmodelmanagementui_LIB_SRCS speechmodelmanagementuiconfiguration.kcfgc)
kde4_add_library(simonmodelmanagementuikcm MODULE ${kcm_simonmodelmanagementui_LIB_SRCS})
target_link_libraries(simonmodelmanagementuikcm  KF5::Codecs  KF5::KIOCore simonscenarios)
install(TARGETS simonmodelmanagementuikcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simon)
install( FILES simonmodelextensionconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
