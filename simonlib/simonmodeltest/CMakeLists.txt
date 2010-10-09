include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/../
	${CMAKE_CURRENT_SOURCE_DIR}/../../julius/libjulius/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../julius/libsent/include)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -D__INTERLOCKED_DECLARED -DKDEWIN_STRINGS_H)
include(KDE4Defaults)

set( simonmodeltest_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modeltest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recognizerresult.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fileresultmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresult.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresultmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresultinstance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresultleaf.cpp
)


set( simonmodeltest_LIB_HDRS
  simonmodeltest_export.h
  modeltest.h
  fileresultmodel.h
  recognizerresult.h
)

kde4_add_library( simonmodeltest  SHARED ${simonmodeltest_LIB_SRCS} )
target_link_libraries( simonmodeltest ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} simonlogging 
	julius sent simonrecognitionresult)


set_target_properties( simonmodeltest
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodeltest_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonmodeltest
  COMPONENT simonddevel
)
 
install( TARGETS simonmodeltest DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT sam  )

add_subdirectory(test)
