message ( "Processing SimonLib..." )

unix {

include($$PWD/KDELib/KDELib.pri)

}

win32 {

include($$PWD/WindowsLib/WindowsLib.pri)

}


include($$PWD/FileSystem/FileSystem.pri)
include($$PWD/Icons/Icons.pri)
include($$PWD/INI/INI.pri)
include($$PWD/Logging/Logging.pri)
include($$PWD/EventSimulation/EventSimulation.pri)
include($$PWD/PasswordDialog/PasswordDialog.pri)
include($$PWD/PostProcessing/PostProcessing.pri)
include($$PWD/ProgramManager/ProgramManager.pri)
include($$PWD/QuickDownloader/QuickDownloader.pri)
include($$PWD/QuickUnpacker/QuickUnpacker.pri)
include($$PWD/Settings/Settings.pri)
include($$PWD/ShortcutButton/ShortcutButton.pri)
include($$PWD/SimonInfo/SimonInfo.pri)
include($$PWD/Sound/Sound.pri)
include($$PWD/SpeechGUI/SpeechGUI.pri)
include($$PWD/TrayIcon/TrayIcon.pri)
include($$PWD/XML/XML.pri)
