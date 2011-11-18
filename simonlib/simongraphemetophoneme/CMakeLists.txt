include(SimonDirs)
include(KDE4Defaults)

set( simongraphemetophoneme_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/graphemetophoneme.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/transcriptionresult.cpp
)

set( simongraphemetophoneme_LIB_HDRS
  simongraphemetophoneme_export.h
  graphemetophoneme.h
)

kde4_add_library( simongraphemetophoneme  SHARED ${simongraphemetophoneme_LIB_SRCS} )
target_link_libraries( simongraphemetophoneme ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( simongraphemetophoneme
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simongraphemetophoneme_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simongraphemetophoneme
  COMPONENT simoncoredevel
)
 
install( TARGETS simongraphemetophoneme DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
