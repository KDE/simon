set(simoncontextcoreui_LIB_SRCS
  newcondition.cpp
  compoundconditionsettings.cpp
)

set(simoncontextcoreui_LIB_HDRS
  newcondition.h
  compoundconditionsettings.h
  simoncontextcoreui_export.h
)

kde4_add_ui_files(simoncontextcoreui_LIB_SRCS
  modifycondition.ui
  compoundconditionsettings.ui
)

kde4_add_library(simoncontextcoreui SHARED ${simoncontextcoreui_LIB_SRCS})

target_link_libraries(simoncontextcoreui ${QT_LIBRARIES} ${QT_QTXML_LIBRARY}
  ${KDE4_KDEUI_LIBS} simoncontextdetection)

set_target_properties( simoncontextcoreui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simoncontextcoreui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simoncontextcoreui COMPONENT simondevel
)

install(TARGETS simoncontextcoreui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)