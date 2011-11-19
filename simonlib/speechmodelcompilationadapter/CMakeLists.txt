
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonmodelcompilationadapter_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadapter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadapterhtk.cpp
)


set( simonmodelcompilationadapter_LIB_HDRS
  simonmodelcompilationadapter_export.h
  modelcompilationadapter.h
  modelcompilationadapterhtk.h
)

kde4_add_library( simonmodelcompilationadapter  SHARED
	${simonmodelcompilationadapter_LIB_SRCS} )
target_link_libraries( simonmodelcompilationadapter ${QT_LIBRARIES}
	${KDE4_KDECORE_LIBS} simonlogging simonscenarios)


set_target_properties( simonmodelcompilationadapter
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonmodelcompilationadapter_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelcompilation
  COMPONENT simonddevel
)
 
install( TARGETS simonmodelcompilationadapter DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )
