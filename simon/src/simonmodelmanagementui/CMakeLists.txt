include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib/speechmodelmanagement ${KCFG_INSTALL_DIR} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonmodelmanagementui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/deleteworddialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dragtablewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/droplistwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/internetextensionsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wordlistview.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/xmltrainingtextlist.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextremotepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextselectsourcepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextlocalpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtexts.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdirectoryworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdata.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdirectoryintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordresolvepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordrecordpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictselectsourcepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictwiktionarypage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importlexiconpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/bompdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictbomppage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/wiktionarydict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/lexicondict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/dict.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminals.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsselectterminalspage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalswizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarwizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarselectfilespage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalwizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalselectparameterspage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminal.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/grammarsettings.cpp
)

set( simonmodelmanagementui_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/wordlistview.h
	${CMAKE_CURRENT_SOURCE_DIR}/trainingview.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagementui_export.h
)

# kde4_add_kcfg_files(simonmodelmanagementui_LIB_SRCS
# 	  speechmodelmanagementconfiguration.kcfgc)

kde4_add_ui_files( simonmodelmanagementui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/internetextensiondlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/removeword.ui
	${CMAKE_CURRENT_SOURCE_DIR}/trainmain.ui
	${CMAKE_CURRENT_SOURCE_DIR}/wordlist.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextlocalpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextremotepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingTexts/importtrainingtextselectsourcepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdataintrodlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportTrainingData/importtrainingdataworkingdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordintropage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/addwordrecordpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/AddWord/resolvewordpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictbomppage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictlexiconpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importdictselectsourcepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportDict/importwiktionarypage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/MergeTerminals/mergeterminalsselectpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammardescriptionpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/selectfilestoreadpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ImportGrammar/importgrammarworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalselectparameterspage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/RenameTerminal/renameterminalworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/grammarsettingsdlg.ui
)

kde4_add_library( simonmodelmanagementui  SHARED ${simonmodelmanagementui_LIB_SRCS} )

target_link_libraries( simonmodelmanagementui ${QT_LIBRARIES}
${KDE4_KDEUI_LIBS} speechgui simonmodelmanagement simoninfo
simonsound )


set_target_properties( simonmodelmanagementui
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodelmanagementui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonmodelmanagementui
  COMPONENT Devel
)
 
install( TARGETS simonmodelmanagementui  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simonmodelinternetextensionconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES simongrammarconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
