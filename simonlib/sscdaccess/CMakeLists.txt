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
target_link_libraries(sscdaccess ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARY} ${KDE4_KDEUI_LIBS} sscobjects)


set_target_properties(sscdaccess
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install(FILES ${sscdaccess_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscdaccess COMPONENT sscdevel)
 
install(TARGETS sscdaccess DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT ssc)
