include(SimonDirs)
find_package(Qt4 COMPONENTS QtCore REQUIRED)

include_directories( ${QT_INCLUDES} ../ ../../)

add_definitions (${QT_DEFINITIONS})

set( sscobjects_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/user.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/microphone.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/soundcard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/language.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sscobject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/institution.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/userininstitution.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sample.cpp
	#${CMAKE_CURRENT_SOURCE_DIR}/sampletype.cpp
)


set( sscobjects_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/sscobjects_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/user.h
  ${CMAKE_CURRENT_SOURCE_DIR}/microphone.h
  ${CMAKE_CURRENT_SOURCE_DIR}/soundcard.h
  ${CMAKE_CURRENT_SOURCE_DIR}/language.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sscobject.h
  ${CMAKE_CURRENT_SOURCE_DIR}/institution.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userininstitution.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sample.h
  #${CMAKE_CURRENT_SOURCE_DIR}/sampletype.h
)

kde4_add_library( sscobjects  SHARED ${sscobjects_LIB_SRCS} )
target_link_libraries( sscobjects ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} )


set_target_properties( sscobjects
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${sscobjects_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/ssc/sscobjects
  COMPONENT sscdevel
)
 
install( TARGETS sscobjects DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT ssc )
