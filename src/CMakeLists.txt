project (simon)
find_package(KDE4 REQUIRED)

include(FindKDE4)
include(FindPortaudio)
include(KDE4Defaults)
include(FindQt4)

set (QT_USE_QTMAIN true)
set (QT_USE_QTNETWORK true)
set (QT_USE_QTDBUS true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})

FIND_LIBRARY(X11_XTest_LIB Xtst ${X11_LIB_SEARCH_PATH})

include_directories( ${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR} ${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${PORTAUDIO_INCLUDE_DIR} SimonLib/SpeechGUI/ )
#add_definitions( ${KDE4_DEFINITIONS} )

IF(${CMAKE_SYSTEM} MATCHES "Linux" )
	add_definitions( -Dlinux )
ENDIF(${CMAKE_SYSTEM} MATCHES "Linux" )

IF(WIN32)
	add_definitions( -D__WIN32 )
ENDIF(WIN32)


set (	simonGeneralSources
		ModelManagement/modelmanager.cpp
		ModelManagement/modelsettings.cpp
		ModelManagement/Grammar/grammarmanager.cpp
		ModelManagement/Grammar/MergeTerminals/mergeterminals.cpp
		ModelManagement/Grammar/MergeTerminals/mergeterminalsworkingpage.cpp
		ModelManagement/Grammar/MergeTerminals/mergeterminalsselectterminalspage.cpp
		ModelManagement/Grammar/MergeTerminals/mergeterminalswizard.cpp
		ModelManagement/Grammar/ImportGrammar/importgrammarwizard.cpp
		ModelManagement/Grammar/ImportGrammar/importgrammarworkingpage.cpp
		ModelManagement/Grammar/ImportGrammar/importgrammarselectfilespage.cpp
		ModelManagement/Grammar/ImportGrammar/importgrammar.cpp
		ModelManagement/Grammar/grammarsettings.cpp
		ModelManagement/Grammar/RenameTerminal/renameterminalwizard.cpp
		ModelManagement/Grammar/RenameTerminal/renameterminalworkingpage.cpp
		ModelManagement/Grammar/RenameTerminal/renameterminalselectparameterspage.cpp
		ModelManagement/Grammar/RenameTerminal/renameterminal.cpp
		ModelManagement/Training/xmltrainingtext.cpp
		ModelManagement/Training/trainingview.cpp
		ModelManagement/Training/trainingtext.cpp
		ModelManagement/Training/trainingmanager.cpp
		ModelManagement/Training/ImportTrainingTexts/xmltrainingtextlist.cpp
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextremotepage.cpp
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextselectsourcepage.cpp
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextlocalpage.cpp
		ModelManagement/Training/ImportTrainingTexts/importtrainingtexts.cpp
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextworkingpage.cpp
		ModelManagement/Training/ImportTrainingData/importtrainingdirectory.cpp
		ModelManagement/Training/ImportTrainingData/importtrainingdirectoryworkingpage.cpp
		ModelManagement/Training/ImportTrainingData/importtrainingdata.cpp
		ModelManagement/Training/ImportTrainingData/importtrainingdirectoryintropage.cpp
		ModelManagement/WordList/wordlistview.cpp
		ModelManagement/WordList/RemoveWord/deleteworddialog.cpp
		ModelManagement/WordList/dragtablewidget.cpp
		ModelManagement/WordList/AddWord/addwordview.cpp
		ModelManagement/WordList/AddWord/addwordresolvepage.cpp
		ModelManagement/WordList/AddWord/addwordintropage.cpp
		ModelManagement/WordList/AddWord/addwordrecordpage.cpp
		ModelManagement/WordList/droplistwidget.cpp
		ModelManagement/WordList/ImportDict/importdictselectsourcepage.cpp
		ModelManagement/WordList/ImportDict/importdictwiktionarypage.cpp
		ModelManagement/WordList/ImportDict/importlexiconpage.cpp
		ModelManagement/WordList/ImportDict/importdict.cpp
		ModelManagement/WordList/ImportDict/importdictworkingpage.cpp
		ModelManagement/WordList/ImportDict/bompdict.cpp
		ModelManagement/WordList/ImportDict/importdictview.cpp
		ModelManagement/WordList/ImportDict/importbomppage.cpp
		ModelManagement/WordList/ImportDict/wiktionarydict.cpp
		ModelManagement/WordList/ImportDict/lexicondict.cpp
		ModelManagement/WordList/ImportDict/dict.cpp
		ModelManagement/WordList/wordlistmanager.cpp
		simonview.cpp
		RecognitionControl/juliuscontrol.cpp
		Actions/commandpreviewwidget.cpp
		Actions/newcommand.cpp
		Actions/actionmanager.cpp
		Actions/Commands/Shortcut/shortcutcommandmanager.cpp
		Actions/Commands/Shortcut/xmlshortcutcommand.cpp
		Actions/Commands/Shortcut/shortcutcommand.cpp
		Actions/Commands/commandmanager.cpp
		Actions/Commands/Place/xmlplacecommand.cpp
		Actions/Commands/Place/placecommand.cpp
		Actions/Commands/Place/ImportPlace/selectplacepage.cpp
		Actions/Commands/Place/ImportPlace/importplacewizard.cpp
		Actions/Commands/Place/placecommandmanager.cpp
		Actions/Commands/commandmodel.cpp
		Actions/Commands/Executable/xmlexecutablecommand.cpp
		Actions/Commands/Executable/executablecommand.cpp
		Actions/Commands/Executable/executablecommandmanager.cpp
		Actions/Commands/Executable/ImportProgram/selectprogrampage.cpp
		Actions/Commands/Executable/ImportProgram/importprogramwizard.cpp
		Actions/Commands/DesktopGrid/desktopgridcommandmanager.cpp
		Actions/Commands/DesktopGrid/screengrid.cpp
		Actions/Commands/ATIntegration/atbackend.cpp
		Actions/Commands/ATIntegration/atobject.cpp
		Actions/Commands/ATIntegration/atwatcher.cpp
		Actions/Commands/commandsettings.cpp
		Actions/Commands/TextMacro/xmltextmacrocommand.cpp
		Actions/Commands/TextMacro/textmacrocommandmanager.cpp
		Actions/Commands/TextMacro/textmacrocommand.cpp
		Actions/Commands/command.cpp
		Actions/runcommandview.cpp
		SimonLib/Sound/wavrecorder.cpp
		SimonLib/Sound/soundsettings.cpp
		SimonLib/Sound/recwidget.cpp
		SimonLib/Sound/wavplayer.cpp
		SimonLib/Sound/soundcontrol.cpp
		SimonLib/Sound/wav.cpp
                SimonLib/SpeechGUI/simonfileselector.cpp
                SimonLib/SpeechGUI/inlinewidgetview.cpp
                SimonLib/SpeechGUI/systemwidgetpage.cpp
                SimonLib/SpeechGUI/systemwidget.cpp
                SimonLib/SpeechGUI/simonlineedit.cpp
                SimonLib/SpeechGUI/simondirselector.cpp
                SimonLib/SpeechGUI/simontablewidget.cpp
                SimonLib/SpeechGUI/simonspinbox.cpp
                SimonLib/SpeechGUI/simongroupbox.cpp
                SimonLib/SpeechGUI/simonfilesystemselector.cpp
                SimonLib/SpeechGUI/simonslider.cpp
                SimonLib/SpeechGUI/simonwidget.cpp
                SimonLib/SpeechGUI/simoncombobox.cpp
                SimonLib/SpeechGUI/simoncalendarwidget.cpp
                SimonLib/SpeechGUI/guievents.cpp
                SimonLib/SpeechGUI/simonmainwindow.cpp
                SimonLib/SpeechGUI/returntablewidget.cpp
                SimonLib/SpeechGUI/inlinewidget.cpp
                SimonLib/SpeechGUI/simonlistwidget.cpp
                SimonLib/SpeechGUI/simontabwidget.cpp
		SimonLib/PasswordDialog/passworddlg.cpp
		SimonLib/Logging/logmanager.cpp
		SimonLib/Logging/logger.cpp
		SimonLib/Logging/logentry.cpp
		SimonLib/ProgramManager/kdeprogrammanager.cpp
		SimonLib/ProgramManager/programmanager.cpp
		SimonLib/ProgramManager/categoryxmlreader.cpp
		SimonLib/QuickDownloader/quickdownloader.cpp
		SimonLib/XML/xmlreader.cpp
		SimonLib/XML/xmldomreader.cpp
		SimonLib/XML/xmlsaxreader.cpp
		SimonLib/TrayIcon/trayiconmanager.cpp
		SimonLib/KDELib/desktopreader.cpp
		SimonLib/EventSimulation/eventhandler.cpp
		SimonLib/EventSimulation/coreevents.cpp
		SimonLib/EventSimulation/shortcut.cpp
		SimonLib/Settings/settings.cpp
		SimonLib/SimonInfo/osd.cpp
		SimonLib/SimonInfo/simoninfo.cpp
		SimonLib/FileSystem/filesystemencoder.cpp
		SimonLib/INI/inireader.cpp
		SimonLib/PostProcessing/postprocessing.cpp
		SimonLib/QuickUnpacker/quickunpacker.cpp
		SimonLib/QuickUnpacker/bunzip.cpp
		SimonLib/QuickUnpacker/compression.cpp
		Configuration/externalprogrammanager.cpp
		Configuration/passwordsettings.cpp
		Configuration/generalsettings.cpp
		Configuration/systemview.cpp
		Configuration/logview.cpp
		Configuration/FirstRun/firstrunmodelsettings.cpp
		Configuration/FirstRun/firstrunwizard.cpp
		Configuration/FirstRun/firstrunimportdictselectsourcepage.cpp
		Configuration/FirstRun/selectlanguagemodelsourcepage.cpp
		Configuration/FirstRun/firstrunimportdictwiktionarypage.cpp
		Configuration/internetextensionsettings.cpp
		Configuration/networksettings.cpp
		simoncontrol.cpp
		main.cpp
)

kde4_add_ui_files( simonGeneralSources 
		ModelManagement/modelsettingsdlg.ui
		ModelManagement/Grammar/MergeTerminals/mergeterminalsworkingpage.ui
		ModelManagement/Grammar/MergeTerminals/mergeterminalsselectpage.ui
		ModelManagement/Grammar/ImportGrammar/importgrammardescriptionpage.ui
		ModelManagement/Grammar/ImportGrammar/selectfilestoreadpage.ui
		ModelManagement/Grammar/ImportGrammar/importgrammarworkingpage.ui
		ModelManagement/Grammar/RenameTerminal/renameterminalselectparameterspage.ui
		ModelManagement/Grammar/RenameTerminal/renameterminalworkingpage.ui
		ModelManagement/Grammar/grammarsettingsdlg.ui
		ModelManagement/Training/trainmain.ui
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextlocalpage.ui
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextremotepage.ui
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextworkingpage.ui
		ModelManagement/Training/ImportTrainingTexts/importtrainingtextselectsourcepage.ui
		ModelManagement/WordList/RemoveWord/removeword.ui
		ModelManagement/WordList/AddWord/resolvewordpage.ui
		ModelManagement/WordList/ImportDict/importbomppage.ui
		ModelManagement/WordList/ImportDict/importdictlexiconpage.ui
		ModelManagement/WordList/ImportDict/importwiktionarypage.ui
		ModelManagement/WordList/wordlist.ui
		Actions/rundialog.ui
		Actions/modifycommands.ui
		Actions/commandpreviewwidget.ui
		Actions/Commands/commandsettingsdlg.ui
		Actions/Commands/Place/ImportPlace/selectplacedlg.ui
		Actions/Commands/Executable/ImportProgram/selectprogramdlg.ui
		SimonLib/Sound/soundsettingsdlg.ui
		SimonLib/Sound/recwidget.ui
		main.ui
		Configuration/passwordsettingsdlg.ui
		Configuration/logmanagerdlg.ui
		Configuration/internetextensiondlg.ui
		Configuration/externalprogramms.ui
		Configuration/generalsettingsdlg.ui
		Configuration/networksettingsdlg.ui
		Configuration/FirstRun/selectlanguagemodelsource.ui
		Configuration/systemviewdlg.ui
		)

set ( simonLinuxSources
		SimonLib/EventSimulation/xevents.cpp
		Actions/Commands/ATIntegration/dbusbackend.cpp
	)

set ( simonWindowsSources
		SimonLib/ProgramManager/windowsprogrammanager.cpp
		SimonLib/WindowsLib/windowsresourcehandler.cpp
		SimonLib/WindowsLib/registrymanager.cpp
		SimonLib/EventSimulation/windowsevents.cpp
		Actions/Commands/ATIntegration/msaabackend.cpp
	)

set ( simon_RCC_SRCS
		banners.qrc
		others.qrc
		action_icons.qrc
		application_icons.qrc
		category_icons.qrc 
		other_icons.qrc
	)


#add_directory(SimonLib/SpeechGUI)

QT4_ADD_RESOURCES(simonGeneralSources ${simon_RCC_SRCS})


IF(UNIX)
	MESSAGE(STATUS "UNIX detected! Congratulations!")
	kde4_add_executable( simon ${simonGeneralSources} ${simonLinuxSources} )
ENDIF(UNIX)

IF(WIN32)
	MESSAGE(STATUS "Windows detected! We are awefully sorry...")
	kde4_add_executable( simon ${simonGeneralSources} ${simonWindowsSources} )
ENDIF(WIN32)



target_link_libraries( simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_LIBRARIES} ${PORTAUDIO_LIBRARIES} )

install (TARGETS simon DESTINATION ${BIN_INSTALL_DIR})
install(FILES simonui.rc simon.log DESTINATION ${DATA_INSTALL_DIR}/simon)
# install(DIRECTORIES model DESTINATION ${DATA_INSTALL_DIR}/simon)

