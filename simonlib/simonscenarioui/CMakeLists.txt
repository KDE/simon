set(simonscenarioui_LIB_SRCS
  scenariomanagementdialog.cpp
  scenariomanagementwidget.cpp
  newscenario.cpp
  newauthor.cpp
)

set(simonscenarioui_LIB_HDRS scenariomanagementdialog.h scenariomanagementwidget.h)

ki18n_wrap_ui(simonscenarioui_LIB_SRCS
  modifyscenario.ui
  modifyauthor.ui
  scenariomanagementdlg.ui
)

kde4_add_library(simonscenarioui SHARED ${simonscenarioui_LIB_SRCS})

target_link_libraries(simonscenarioui ${QT_LIBRARIES}
  simonscenarios simonscenariobase KF5::KIOCore KF5::NewStuff)


set_target_properties(simonscenarioui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonscenarioui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenarioui
  COMPONENT simondevel
)
 
install(TARGETS simonscenarioui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
