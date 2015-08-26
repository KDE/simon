set(simoncontextui_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/contextview.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/contextviewprivate.cpp
)

set(simoncontextui_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/contextview.h
  ${CMAKE_CURRENT_SOURCE_DIR}/simoncontextui_export.h
)

ki18n_wrap_ui(simoncontextui_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/contextdialog.ui
)

kde4_add_library(simoncontextui SHARED ${simoncontextui_LIB_SRCS})

target_link_libraries(simoncontextui ${QT_LIBRARIES}  
  KF5::KIOCore  simonactions simonscenarios 
  simonscenarios simonuicomponents simoncontextdetection simoncontextcoreui
  simonscenarioui)

set_target_properties(simoncontextui PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simoncontextui_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simoncontextui COMPONENT simondevel)
install(TARGETS simoncontextui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)
