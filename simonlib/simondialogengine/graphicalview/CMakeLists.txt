set (simondialogenginegraphicalview_SRCS 
  visualdialogview.cpp
  dialogcommandbutton.cpp
)

set(simondialogenginegraphicalview_LIB_HDRS
  visualdialogview.h
  simondialogenginevisualview_export.h
)

kde4_add_ui_files(simondialogenginegraphicalview_SRCS 
  dialogwidget.ui
)

kde4_add_library(simondialogenginegraphicalview SHARED ${simondialogenginegraphicalview_SRCS})

target_link_libraries(simondialogenginegraphicalview 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simonactions simondialogengine simonscenarios)

set_target_properties(simondialogenginegraphicalview
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(TARGETS simondialogenginegraphicalview DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)

install(FILES ${simondialogenginegraphicalview_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine/graphicalview
  COMPONENT simondevel
)
