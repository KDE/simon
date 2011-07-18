include(SimonDirs)
find_package(Qt4 COMPONENTS QtCore QtNetwork REQUIRED)

include_directories( ${QT_INCLUDES} ../ ../../)

add_definitions (${QT_DEFINITIONS})

set( sscdaccess_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/sscdaccess.cpp
)

set( sscdaccess_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/sscdaccess.h
)

kde4_add_library( sscdaccess  SHARED ${sscdaccess_LIB_SRCS} )
target_link_libraries( sscdaccess ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${KDE_LIBS} ${KDE4_KDEUI_LIBS} sscobjects)


set_target_properties( sscdaccess
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${sscdaccess_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscdaccess #i'm not sure about that destination
  COMPONENT sscdevel
)
 
install( TARGETS sscdaccess DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT ssc )#and even of this component
