include(FindQt4)


set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTDBUS true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})


include_directories( ${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR} ${KDE4_INCLUDE_DIR}/KDE 
		     ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib
		     ${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib/speechgui
		     ${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList
		     ${CMAKE_CURRENT_SOURCE_DIR}/AppCore
		     ${CMAKE_CURRENT_SOURCE_DIR}/Actions
		     ${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Actions)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/AppCore)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/RecognitionControl)

set (	simonGeneralSources
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/trainingview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/xmltrainingtextlist.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextremotepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextselectsourcepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextlocalpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtexts.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingData/importtrainingdirectory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingData/importtrainingdirectoryworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingData/importtrainingdata.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingData/importtrainingdirectoryintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/wordlistview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/RemoveWord/deleteworddialog.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/dragtablewidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/addwordview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/addwordresolvepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/addwordintropage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/addwordrecordpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/droplistwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictselectsourcepage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictwiktionarypage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importlexiconpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictworkingpage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/bompdict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictbomppage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/wiktionarydict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/lexicondict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/dict.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/internetextensionsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Actions/commandpreviewwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Actions/newcommand.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Actions/runcommandview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

kde4_add_ui_files( simonGeneralSources 
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/trainmain.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextlocalpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextremotepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextworkingpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingTexts/importtrainingtextselectsourcepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingData/importtrainingdataintrodlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/ImportTrainingData/importtrainingdataworkingdlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/RemoveWord/removeword.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/addwordintropage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/addwordrecordpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/AddWord/resolvewordpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictbomppage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictlexiconpage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importdictselectsourcepage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/ImportDict/importwiktionarypage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/wordlist.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/internetextensiondlg.ui
	${CMAKE_CURRENT_SOURCE_DIR}/Actions/rundialog.ui
	${CMAKE_CURRENT_SOURCE_DIR}/Actions/modifycommands.ui
	${CMAKE_CURRENT_SOURCE_DIR}/Actions/commandpreviewwidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/simonview.ui
)


install( FILES ${simon_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon
  COMPONENT Devel
)


kde4_add_app_icon(simonGeneralSources "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simon.png")

IF(UNIX)
	MESSAGE(STATUS "UNIX detected! Congratulations!")
	kde4_add_executable( simon ${simonGeneralSources} )
ENDIF(UNIX)

IF(WIN32)
	MESSAGE(STATUS "Windows detected! We are awfully sorry...")
	kde4_add_executable( simon ${simonGeneralSources} )
ENDIF(WIN32)


target_link_libraries( simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
${QT_LIBRARIES} ${KDE4_KUTILS_LIBS} simonappcore simoninfo
simonlogging simonxml speechgui simonsound simonmodelmanagement
simonmodelmanagement simonrecognitioncontrol simonactions)

install (TARGETS simon DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES simonui.rc simon.log DESTINATION ${DATA_INSTALL_DIR})
