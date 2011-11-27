
include(KDE4Macros)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib/simonscenarios ${KCFG_INSTALL_DIR} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DMAKE_SIMONMODELMANAGEMENTUI_LIB)
include(KDE4Defaults)

set( simonmodelmanagementui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelmanageruiproxy.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/deleteworddialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/editworddialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dragtableview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/droplistwidget.cpp
# 	${CMAKE_CURRENT_SOURCE_DIR}/modelextensionsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingviewprivate.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/xmltrainingtextlist.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextselectsourcepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextlocalpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtexts.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextaddpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdirectoryworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdata.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdirectoryintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordresolvepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordrecordpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictselectsourcepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importlexiconpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/bompdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/sphinxdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictbomppage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictbompdownloadpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictsphinxpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictjuliuspage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictplspage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/lexicondict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/plsdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/juliusvocabulary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/dict.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminals.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsselectterminalspage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalswizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarwizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarselectinputpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalwizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalselectparameterspage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TrainSamples/trainingswizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TrainSamples/trainsampleintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TrainSamples/trainsamplepage.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/CreateLanguageProfile/languageprofileview.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/grammarview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/grammarviewprivate.cpp
)

kde4_add_ui_files( simonmodelmanagementui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelextensionsettingsdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/removeword.ui
	${CMAKE_CURRENT_SOURCE_DIR}/editword.ui
	${CMAKE_CURRENT_SOURCE_DIR}/trainmain.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextlocalpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextaddpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextselectsourcepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdataintrodlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdataworkingdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordintropage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/resolvewordpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictbomppage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictbompdownloadpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictsphinxpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictjuliuspage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictplspage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictlexiconpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictselectsourcepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsselectpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/selectinputpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalselectparameterspage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalworkingpage.ui

	${CMAKE_CURRENT_SOURCE_DIR}/CreateLanguageProfile/languageprofileview.ui

	${CMAKE_CURRENT_SOURCE_DIR}/grammarview.ui
)

set( simonmodelmanagementui_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/trainingview.h
	${CMAKE_CURRENT_SOURCE_DIR}/grammarview.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagementui_export.h
)

set( simonmodelmanagementui_LIB_SRCS
	${simonmodelmanagementui_LIB_SRCS}
	${CMAKE_CURRENT_SOURCE_DIR}/vocabularyview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vocabularyviewprivate.cpp
	)
set( simonmodelmanagementui_LIB_HDRS
	${simonmodelmanagementui_LIB_HDRS}
	${CMAKE_CURRENT_SOURCE_DIR}/vocabularyview.h
	)
kde4_add_ui_files( simonmodelmanagementui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/vocabulary.ui)


 kde4_add_kcfg_files(simonmodelmanagementui_LIB_SRCS
 	  speechmodelmanagementuiconfiguration.kcfgc)


kde4_add_library( simonmodelmanagementui  SHARED ${simonmodelmanagementui_LIB_SRCS} )

target_link_libraries( simonmodelmanagementui ${QT_LIBRARIES}
${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonlogging simonscenarios simoninfo simonxml
simonsound simonuicomponents simongraphemetophoneme)

set_target_properties( simonmodelmanagementui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonmodelmanagementui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonmodelmanagementui
  COMPONENT simondevel
)

install( TARGETS simonmodelmanagementui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )

################

set(kcm_simonmodelmanagementui_LIB_SRCS 
	${CMAKE_CURRENT_SOURCE_DIR}/modelextensionsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
 )

kde4_add_ui_files( kcm_simonmodelmanagementui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelextensionsettingsdlg.ui)

kde4_add_kcfg_files(kcm_simonmodelmanagementui_LIB_SRCS
 	  speechmodelmanagementuiconfiguration.kcfgc)
kde4_add_plugin(simonmodelmanagementuikcm ${kcm_simonmodelmanagementui_LIB_SRCS})
target_link_libraries(simonmodelmanagementuikcm ${KDE4_KDEUI_LIBS}
	${KDE4_KIO_LIBS} simonscenarios
)
install(TARGETS simonmodelmanagementuikcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simon)
install( FILES simonmodelextensionconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
