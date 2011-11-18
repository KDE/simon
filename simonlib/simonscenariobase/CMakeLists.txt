include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ ../../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DMAKE_SCENARIOBASE_LIB)
include(KDE4Defaults)

set( simonscenariobase_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/versionnumber.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scenarioobject.cpp
)


set( simonscenariobase_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/simonscenariobase_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/versionnumber.h
  ${CMAKE_CURRENT_SOURCE_DIR}/scenarioobject.h
)

kde4_add_library( simonscenariobase  SHARED ${simonscenariobase_LIB_SRCS} )
target_link_libraries( simonscenariobase ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${KDE4_KDECORE_LIBS} )


set_target_properties( simonscenariobase
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonscenariobase_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonscenariobase
  COMPONENT simoncoredevel
)
 
install( TARGETS simonscenariobase DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
