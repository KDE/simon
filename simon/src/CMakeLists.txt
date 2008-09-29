include(FindPortaudio)
include(FindQt4)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTDBUS true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})

FIND_LIBRARY(X11_XTest_LIB Xtst ${X11_LIB_SEARCH_PATH})

include_directories( ${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR} ${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${PORTAUDIO_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/ )



set (	simonGeneralSources
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/modelmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/modelsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/grammarmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/MergeTerminals/mergeterminals.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/MergeTerminals/mergeterminalsworkingpage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/MergeTerminals/mergeterminalsselectterminalspage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/MergeTerminals/mergeterminalswizard.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/importgrammarwizard.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/importgrammarworkingpage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/importgrammarselectfilespage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/importgrammar.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/grammarsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/RenameTerminal/renameterminalwizard.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/RenameTerminal/renameterminalworkingpage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/RenameTerminal/renameterminalselectparameterspage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/RenameTerminal/renameterminal.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/xmltrainingtext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/trainingview.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/trainingtext.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Training/trainingmanager.cpp
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
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/WordList/wordlistmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/simonview.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/RecognitionControl/juliuscontrol.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/commandpreviewwidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/newcommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/actionmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Shortcut/shortcutcommandmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Shortcut/xmlshortcutcommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Shortcut/shortcutcommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/commandmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Place/xmlplacecommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Place/placecommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Place/ImportPlace/selectplacepage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Place/ImportPlace/importplacewizard.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Place/placecommandmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/commandmodel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Executable/xmlexecutablecommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Executable/executablecommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Executable/executablecommandmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Executable/ImportProgram/selectprogrampage.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Executable/ImportProgram/importprogramwizard.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/DesktopGrid/desktopgridcommandmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/DesktopGrid/screengrid.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/ATIntegration/atbackend.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/ATIntegration/atobject.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/ATIntegration/atwatcher.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/commandsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/TextMacro/xmltextmacrocommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/TextMacro/textmacrocommandmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/TextMacro/textmacrocommand.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/command.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/runcommandview.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/wavrecorder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/soundsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/recwidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/wavplayer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/soundcontrol.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/wav.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/inlinewidgetview.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simonlineedit.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simontablewidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simonspinbox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simongroupbox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simonslider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simonwidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simoncombobox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simoncalendarwidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/guievents.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simonmainwindow.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/returntablewidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/inlinewidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simonlistwidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SpeechGUI/simontabwidget.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Logging/logmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Logging/logger.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Logging/logentry.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/XML/xmlreader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/XML/xmldomreader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/XML/xmlsaxreader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/TrayIcon/trayiconmanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/EventSimulation/eventhandler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/EventSimulation/coreevents.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SimonInfo/osd.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/SimonInfo/simoninfo.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/FileSystem/filesystemencoder.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/PostProcessing/postprocessing.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/externalprogrammanager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/passwordsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/generalsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/configurationdialog.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Logging/logview.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/internetextensionsettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/RecognitionControl/networksettings.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/ServerAddressSelector/addserverconnection.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/ServerAddressSelector/serveraddressselector.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/simoncontrol.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

kde4_add_ui_files( simonGeneralSources 
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/modelsettingsdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/MergeTerminals/mergeterminalsworkingpage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/MergeTerminals/mergeterminalsselectpage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/importgrammardescriptionpage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/selectfilestoreadpage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/ImportGrammar/importgrammarworkingpage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/RenameTerminal/renameterminalselectparameterspage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/RenameTerminal/renameterminalworkingpage.ui
		${CMAKE_CURRENT_SOURCE_DIR}/ModelManagement/Grammar/grammarsettingsdlg.ui
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
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/rundialog.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/modifycommands.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/commandpreviewwidget.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/commandsettingsdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Place/ImportPlace/selectplacedlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/Executable/ImportProgram/selectprogramdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/soundsettingsdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Sound/recwidget.ui
		${CMAKE_CURRENT_SOURCE_DIR}/main.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/passwordsettingsdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/Logging/logmanagerdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/internetextensiondlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/externalprogramms.ui
		${CMAKE_CURRENT_SOURCE_DIR}/Configuration/generalsettingsdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/RecognitionControl/networksettingsdlg.ui
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/ServerAddressSelector/addserverconnectiondlg.ui
		)

set ( simonLinuxSources
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/EventSimulation/xevents.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/ATIntegration/dbusbackend.cpp
	)

set ( simonWindowsSources
		${CMAKE_CURRENT_SOURCE_DIR}/SimonLib/EventSimulation/windowsevents.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Actions/Commands/ATIntegration/msaabackend.cpp
	)

kde4_add_kcfg_files(simonGeneralSources coreconfiguration.kcfgc)


kde4_add_app_icon(simonGeneralSources "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simon.png")

IF(UNIX)
	MESSAGE(STATUS "UNIX detected! Congratulations!")
	kde4_add_executable( simon ${simonGeneralSources} ${simonLinuxSources} )
ENDIF(UNIX)

IF(WIN32)
	MESSAGE(STATUS "Windows detected! We are awfully sorry...")
	kde4_add_executable( simon ${simonGeneralSources} ${simonWindowsSources} )
ENDIF(WIN32)

install(FILES simon.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

target_link_libraries( simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_LIBRARIES} ${PORTAUDIO_LIBRARIES} )

install (TARGETS simon DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES simonui.rc simon.log DESTINATION ${DATA_INSTALL_DIR}/simon)
