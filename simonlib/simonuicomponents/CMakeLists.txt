set(simonuicomponents_LIB_SRCS
  inlinewidgetview.cpp
  inlinewidget.cpp
  addserverconnection.cpp
  serveraddressselector.cpp
  selectionlistview.cpp
  trayiconmanager.cpp
  textlistwidget.cpp
  simonwizard.cpp
  multikcmview.cpp
  selectprogramdialog.cpp
  actionbutton.cpp
  verticalscrollarea.cpp
)

set(simonuicomponents_LIB_HDRS
  simonuicomponents_export.h
  inlinewidgetview.h
  inlinewidget.h
  addserverconnection.h
  selectionlistview.h
  serveraddressselector.h
  simonwizard.h
  trayiconmanager.h
  multikcmview.h
  textlistwidget.h
  selectprogramdialog.h
  actionbutton.h
  verticalscrollarea.h
)

kde4_add_ui_files(simonuicomponents_LIB_SRCS selectprogramdlg.ui)
kde4_add_library(simonuicomponents SHARED ${simonuicomponents_LIB_SRCS})

target_link_libraries(simonuicomponents ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS})

set_target_properties(simonuicomponents
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonuicomponents_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonuicomponents
  COMPONENT simoncoredevel
)

install(TARGETS simonuicomponents DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
