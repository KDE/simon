set(simongraphemetophoneme_LIB_SRCS
  graphemetophoneme.cpp
  transcriptionresult.cpp
)

set(simongraphemetophoneme_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/simongraphemetophoneme_export.h
  graphemetophoneme.h
)

kde4_add_library(simongraphemetophoneme  SHARED ${simongraphemetophoneme_LIB_SRCS})
target_link_libraries(simongraphemetophoneme ${QT_LIBRARIES} KF5::KDELibs4Support)

generate_export_header(simongraphemetophoneme)

set_target_properties(simongraphemetophoneme
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simongraphemetophoneme_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simongraphemetophoneme
  COMPONENT simoncoredevel
)
 
install(TARGETS simongraphemetophoneme DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
