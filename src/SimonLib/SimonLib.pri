message ( "Processing SimonLib..." )

unix {

include(SimonLib/KDELib/KDELib.pri)

}

win32 {

include(SimonLib/WindowsLib/WindowsLib.pri)

}


include(SimonLib/FileSystem/FileSystem.pri)
include(SimonLib/Icons/Icons.pri)
include(SimonLib/INI/INI.pri)
include(SimonLib/Logging/Logging.pri)
include(SimonLib/EventSimulation/EventSimulation.pri)
include(SimonLib/PasswordDialog/PasswordDialog.pri)
include(SimonLib/PostProcessing/PostProcessing.pri)
include(SimonLib/ProgramManager/ProgramManager.pri)
include(SimonLib/QuickDownloader/QuickDownloader.pri)
include(SimonLib/QuickUnpacker/QuickUnpacker.pri)
include(SimonLib/Settings/Settings.pri)
include(SimonLib/ShortcutButton/ShortcutButton.pri)
include(SimonLib/SimonInfo/SimonInfo.pri)
include(SimonLib/Sound/Sound.pri)
include(SimonLib/SpeechGUI/SpeechGUI.pri)
include(SimonLib/TrayIcon/TrayIcon.pri)
include(SimonLib/XML/XML.pri)
