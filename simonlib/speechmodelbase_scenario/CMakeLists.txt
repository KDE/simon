include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( speechmodelbase_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/wordlistcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/grammarcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/languagedescriptioncontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingcontainer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/model.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingtext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/word.h
)


set( speechmodelbase_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/speechmodelbase_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/word.h
  ${CMAKE_CURRENT_SOURCE_DIR}/wordlistcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/grammarcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/languagedescriptioncontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingcontainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/model.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingtext.h
)

kde4_add_library( speechmodelbase  SHARED ${speechmodelbase_LIB_SRCS} )
target_link_libraries( speechmodelbase ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( speechmodelbase
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${speechmodelbase_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelbase
  COMPONENT simoncoredevel
)
 
install( TARGETS speechmodelbase DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )