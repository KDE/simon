set(simonactionsui_LIB_SRCS
  commandpreviewwidget.cpp
  newcommand.cpp
  manageactionsdialog.cpp
  manageactionsautorunwidget.cpp
  runcommandview.cpp
  runcommandviewprivate.cpp
  addactiondialog.cpp
)

set(simonactionsui_LIB_HDRS runcommandview.h)

ki18n_wrap_ui(simonactionsui_LIB_SRCS
  commandpreviewwidget.ui
  modifycommands.ui
  rundialog.ui
  manageactionsdlg.ui
  manageactionsautorunwidget.ui
  addactiondlg.ui
)

kde4_add_library(simonactionsui  SHARED ${simonactionsui_LIB_SRCS})

target_link_libraries(simonactionsui ${QT_LIBRARIES}
   KF5::KIOCore  simonactions
  simonscenarios simonscenarios simonuicomponents
)

set_target_properties( simonactionsui PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simonactionsui_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonactionsui COMPONENT simondevel)
install(TARGETS simonactionsui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)
