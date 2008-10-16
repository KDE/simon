include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( speechmodelbase_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingtext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/word.h
)


set( speechmodelbase_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/speechmodelbase_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/word.h
  ${CMAKE_CURRENT_SOURCE_DIR}/modelcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingtext.h
)

kde4_add_library( speechmodelbase  SHARED ${speechmodelbase_LIB_SRCS} )
target_link_libraries( speechmodelbase ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( speechmodelbase
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${speechmodelbase_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelbase
  COMPONENT Devel
)
 
install( TARGETS speechmodelbase  ${INSTALL_TARGETS_DEFAULT_ARGS} )