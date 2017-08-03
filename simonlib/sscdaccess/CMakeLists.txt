set(sscdaccess_LIB_SRCS
  sscdaccess.cpp
  sscdaccesssingleton.cpp
  abstractsampledataprovider.cpp
  sendsampleworker.cpp
  trainingsamplesdescriptor.cpp
)

set(sscdaccess_LIB_HDRS
  sscdaccess.h
  sscdaccesssingleton.h
)

kde4_add_library(sscdaccess SHARED ${sscdaccess_LIB_SRCS})
target_link_libraries(sscdaccess sscobjects simonprogresstracking ${QT_LIBRARIES} Qt5::Network KF5::KDELibs4Support)

generate_export_header(sscdaccess)

set_target_properties(sscdaccess
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${sscdaccess_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscdaccess COMPONENT sscdevel)

install(TARGETS sscdaccess DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT ssc)
