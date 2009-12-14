include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../
			${CMAKE_CURRENT_SOURCE_DIR}/..
			${CMAKE_CURRENT_SOURCE_DIR}/../
			${CMAKE_CURRENT_SOURCE_DIR}/../speechgui
)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DMAKE_MODELMANAGEMENT_LIB)
include(KDE4Defaults)

set( simonscenarios_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/trainingmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/speechmodelsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wordlisttype.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/vocabulary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/activevocabulary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shadowvocabulary.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scenario.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scenariomanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/grammar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingtextcollection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingtext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/languagedescriptioncontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/word.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/author.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scenariodisplay.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/commandconfiguration.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/createcommandwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/action.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/actioncollection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/actionmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/actioncommandmodel.cpp

)


set( simonscenarios_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagement_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingmanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/modelmanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/scenario.h
  ${CMAKE_CURRENT_SOURCE_DIR}/activevocabulary.h
  ${CMAKE_CURRENT_SOURCE_DIR}/shadowvocabulary.h
  ${CMAKE_CURRENT_SOURCE_DIR}/wordlisttype.h
  ${CMAKE_CURRENT_SOURCE_DIR}/scenariomanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/grammar.h
  ${CMAKE_CURRENT_SOURCE_DIR}/speechmodel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingtext.h
  ${CMAKE_CURRENT_SOURCE_DIR}/word.h
  ${CMAKE_CURRENT_SOURCE_DIR}/languagedescriptioncontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/model.h


${CMAKE_CURRENT_SOURCE_DIR}/author.h
${CMAKE_CURRENT_SOURCE_DIR}/scenariodisplay.h

	${CMAKE_CURRENT_SOURCE_DIR}/commandmanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/command.h
	${CMAKE_CURRENT_SOURCE_DIR}/commandconfiguration.h
	${CMAKE_CURRENT_SOURCE_DIR}/createcommandwidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/action.h
	${CMAKE_CURRENT_SOURCE_DIR}/actioncollection.h
	${CMAKE_CURRENT_SOURCE_DIR}/actionmodel.h
	${CMAKE_CURRENT_SOURCE_DIR}/actioncommandmodel.h

)



kde4_add_ui_files( simonscenarios_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/trainingsettings.ui
)

kde4_add_kcfg_files(simonscenarios_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/speechmodelmanagementconfiguration.kcfgc
)

kde4_add_library( simonscenarios SHARED ${simonscenarios_LIB_SRCS} )
target_link_libraries( simonscenarios ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} 
	simonxml simonlogging speechgui  simonprogresstracking  
	simonscenariobase simoninfo )


set_target_properties( simonscenarios
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonscenarios_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenarios
  COMPONENT simondevel
)
 
install( TARGETS simonscenarios DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
install( FILES simonspeechmodelmanagementconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/speechmodelmanagementconfiguration.kcfg DESTINATION  ${KCFG_INSTALL_DIR} COMPONENT simon )

install(FILES
${CMAKE_CURRENT_SOURCE_DIR}/simoncommandplugin.desktop DESTINATION
${SERVICETYPES_INSTALL_DIR} COMPONENT simon)
 
	
