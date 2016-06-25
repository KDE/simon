set(sscobjects_LIB_SRCS
  user.cpp
  microphone.cpp
  soundcard.cpp
  language.cpp
  sscobject.cpp
  institution.cpp
  userininstitution.cpp
  sample.cpp
)


set(sscobjects_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/sscobjects_export.h
  user.h
  microphone.h
  soundcard.h
  language.cpp
  sscobject.h
  institution.h
  userininstitution.h
  sample.h
)

kde4_add_library(sscobjects SHARED ${sscobjects_LIB_SRCS})
target_link_libraries(sscobjects ${QT_LIBRARIES} Qt5::Core)

generate_export_header(sscobjects)

set_target_properties(sscobjects
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${sscobjects_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscobjects
  COMPONENT sscdevel
)

install(TARGETS sscobjects DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT ssc)
