set(simonlistcommand_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/listcommand.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/commandlistwidget.cpp
)

set(simonlistcommand_LIB_HDRS
  listcommand.h
  commandlistwidget.h
)

ki18n_wrap_ui(simonlistcommand_LIB_SRCS
  listcommand.ui
)

kde4_add_library(simonlistcommand SHARED ${simonlistcommand_LIB_SRCS})

target_link_libraries(simonlistcommand ${QT_LIBRARIES}  simonactions simonscenarios)

set_target_properties(simonlistcommand
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonlistcommand_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/listcommand COMPONENT simondevel)
install(TARGETS simonlistcommand DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)
