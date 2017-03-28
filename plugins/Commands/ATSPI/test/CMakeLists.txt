include_directories(../)
add_definitions(-DQT_GUI_LIB)
set (simonatspiscannertest_SRC
  atspiscannertest.cpp
  accessibleapp.cpp
  accessibleapp.h

  #deps
  ../atspiscanner.cpp
)

kde4_add_unit_test(simonatspicommandplugin-scanner TESTNAME
  simonatspicommandplugin-scanner
  ${simonatspiscannertest_SRC}
)


target_link_libraries( simonatspicommandplugin-scanner
  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
  ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
  ${QACCESSIBILITYCLIENT_LIBRARY} simonscenarios simonactions)
