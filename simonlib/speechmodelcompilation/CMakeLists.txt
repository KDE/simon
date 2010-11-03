include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ ../../julius/libjulius/include)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -D__INTERLOCKED_DECLARED)
include(KDE4Defaults)

set( simonmodelcompilation_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanager.cpp
)


set( simonmodelcompilation_LIB_HDRS
  simonmodelcompilationmanagement_export.h
  modelcompilationmanager.h
)

kde4_add_library( simonmodelcompilation  SHARED ${simonmodelcompilation_LIB_SRCS} )
target_link_libraries( simonmodelcompilation ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} simonlogging )


set_target_properties( simonmodelcompilation
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodelcompilation_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelcompilation
  COMPONENT simonddevel
)
 
install( TARGETS simonmodelcompilation DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )

add_subdirectory(config)
