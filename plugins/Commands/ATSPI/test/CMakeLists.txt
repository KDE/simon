include_directories(../)
add_definitions(-DQT_GUI_LIB)
set (simonatspiscannertest_SRC
  atspiscannertest.cpp
  accessibleapp.cpp
  accessibleapp.h

  #deps
  ../atspiscanner.cpp
)

add_executable(simonatspicommandplugin-scanner
  simonatspicommandplugin-scanner
  ${simonatspiscannertest_SRC}
)
add_test(simonatspicommandplugin simonatspicommandplugin)
ecm_mark_as_test(simonatspicommandplugin)


target_link_libraries( simonatspicommandplugin-scanner
  KF5::KDELibs4Support Qt5::Test
   Qt5::Xml
  ${QACCESSIBILITYCLIENT_LIBRARY} simonscenarios simonactions)
