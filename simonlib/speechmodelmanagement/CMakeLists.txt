include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../
			${CMAKE_CURRENT_SOURCE_DIR}/..
			${CMAKE_CURRENT_SOURCE_DIR}/../speechmodelbase
			${CMAKE_CURRENT_SOURCE_DIR}/../speechgui
)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DMAKE_MODELMANAGEMENT_LIB)
include(KDE4Defaults)

set( simonmodelmanagement_LIB_SRCS
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
	${CMAKE_CURRENT_SOURCE_DIR}/wordlistcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/grammarcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/languagedescriptioncontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/word.cpp

)


set( simonmodelmanagement_LIB_HDRS
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
  ${CMAKE_CURRENT_SOURCE_DIR}/wordlistcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/grammarcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/languagedescriptioncontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/model.h

)



kde4_add_ui_files( simonmodelmanagement_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/trainingsettings.ui
)

kde4_add_kcfg_files(simonmodelmanagement_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/speechmodelmanagementconfiguration.kcfgc
)

kde4_add_library( simonmodelmanagement SHARED ${simonmodelmanagement_LIB_SRCS} )
target_link_libraries( simonmodelmanagement ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} 
	simonxml simonlogging speechgui speechmodelbase simonprogresstracking  simonactions 
	simonscenariobase simoncommandpluginbase )


set_target_properties( simonmodelmanagement
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodelmanagement_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelmanagement
  COMPONENT simondevel
)
 
install( TARGETS simonmodelmanagement DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
install( FILES simonspeechmodelmanagementconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/speechmodelmanagementconfiguration.kcfg DESTINATION  ${KCFG_INSTALL_DIR} COMPONENT simon )

	
