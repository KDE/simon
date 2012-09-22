
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -D__INTERLOCKED_DECLARED)
include(KDE4Defaults)

set( simonmodelcompilation_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanagerhtk.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadapter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadapterhtk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilerhtk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompiler.cpp
)


set( simonmodelcompilation_LIB_HDRS
  simonmodelcompilationmanagement_export.h
  modelcompilationmanager.h
  modelcompiler.h
  modelcompilerhtk.h
  modelcompilation.h
  modelcompilationadapter.h
  modelcompilationadapterhtk.h
)

kde4_add_library( simonmodelcompilation  SHARED ${simonmodelcompilation_LIB_SRCS} )
target_link_libraries( simonmodelcompilation ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${KDE4_KDECORE_LIBS} simonlogging simonscenarios simonscenariobase )


set_target_properties( simonmodelcompilation
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonmodelcompilation_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelcompilation
  COMPONENT simonddevel
)
 
install( TARGETS simonmodelcompilation DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )

add_subdirectory(config)
