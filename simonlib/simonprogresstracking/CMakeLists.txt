set(simonprogresstracking_LIB_SRCS
  operation.cpp
  statusmanager.cpp
  progresswidget.cpp
  compositeprogresswidget.cpp
)

set(simonprogresstracking_LIB_HDRS
  simonprogresstracking_export.h
  statusmanager.h
  operation.h
)

kde4_add_library(simonprogresstracking SHARED ${simonprogresstracking_LIB_SRCS})
target_link_libraries(simonprogresstracking ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS})

set_target_properties(simonprogresstracking
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonprogresstracking_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonprogresstracking
  COMPONENT simondevel
)

install(TARGETS simonprogresstracking DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)