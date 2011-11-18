find_package(KDE4 REQUIRED)
include(SimonDirs)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES}
${CMAKE_CURRENT_SOURCE_DIR}/../..)

set (simondialogenginettsview_SRCS 
  ttsdialogview.cpp
    )

set( simondialogenginettsview_LIB_HDRS
  ttsdialogview.h
  simondialogenginettsview_export.h
)

kde4_add_library(simondialogenginettsview SHARED ${simondialogenginettsview_SRCS})
set_target_properties( simondialogenginettsview
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

target_link_libraries(simondialogenginettsview 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simontts simondialogengine)

install(TARGETS simondialogenginettsview DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )

install( FILES ${simondialogenginettsview_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine/ttsview
  COMPONENT simondevel
)
