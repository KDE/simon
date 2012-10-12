set(simondialogengineconfui_SRCS 
  createtemplateoptiondialog.cpp
  createboundvaluedialog.cpp 
  createavatardialog.cpp
  avatarconfiguration.cpp
  boundvaluesconfiguration.cpp
  templateoptionsconfiguration.cpp
  outputconfiguration.cpp
)

set(simondialogengineconfui_LIB_HDRS
  createtemplateoptiondialog.h
  createboundvaluedialog.h
  createavatardialog.h
  avatarconfiguration.h
  boundvaluesconfiguration.h
  templateoptionsconfiguration.h
  outputconfiguration.h
  simondialogengineconfui_export.h
)

kde4_add_ui_files(simondialogengineconfui_SRCS 
  createtemplateoptiondialog.ui
  createboundvalue.ui
  createavatardlg.ui
  avatarconfiguration.ui
  boundvaluesconfiguration.ui
  outputconfiguration.ui
  templateoptionsconfiguration.ui
)

kde4_add_library(simondialogengineconfui SHARED ${simondialogengineconfui_SRCS})
set_target_properties( simondialogengineconfui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

if (USE_PLASMA)
  target_link_libraries(simondialogengineconfui ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS} simondialogengine)
else ()
  target_link_libraries(simondialogengineconfui ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simondialogengine)
endif()

install(TARGETS simondialogengineconfui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
install(FILES ${simondialogengineconfui_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine/confui COMPONENT simondevel)
