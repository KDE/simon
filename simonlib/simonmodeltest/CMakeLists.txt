
include(SimonDirs)

if(${BackendType} STREQUAL both)
find_package(Sphinxbase REQUIRED)
find_package(Pocketsphinx REQUIRED)
include_directories(${POCKETSPHINX_INCLUDE_DIR} ${SphinxBase_INCLUDE_DIR})
endif(${BackendType} STREQUAL both)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -D__INTERLOCKED_DECLARED -DKDEWIN_STRINGS_H)
include(KDE4Defaults)

set( simonmodeltest_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/modeltest.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusmodeltest.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recognizerresult.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fileresultmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresult.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresultmodel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresultinstance.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/testresultleaf.cpp
)

if(${BackendType} STREQUAL both)
set( simonmodeltest_LIB_SRCS
     ${simonmodeltest_LIB_SRCS}
     ${CMAKE_CURRENT_SOURCE_DIR}/sphinxmodeltest.cpp
    )
endif(${BackendType} STREQUAL both)

set( simonmodeltest_LIB_HDRS
  simonmodeltest_export.h
  modeltest.h
  juliusmodeltest.h
  fileresultmodel.h
  recognizerresult.h
)

if(${BackendType} STREQUAL both)
set( simonmodeltest_LIB_HDRS
     ${simonmodeltest_LIB_HDRS}
     sphinxmodeltest.h
    )
endif(${BackendType} STREQUAL both)

kde4_add_library( simonmodeltest  SHARED ${simonmodeltest_LIB_SRCS} )
target_link_libraries( simonmodeltest ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} simonlogging 
	simonrecognitionresult simonrecognizer)


set_target_properties( simonmodeltest
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonmodeltest_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonmodeltest
  COMPONENT simonddevel
)

#set_target_properties(simonmodeltest PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Winit-self -Wunreachable-code")

install( TARGETS simonmodeltest DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT sam  )

add_subdirectory(test)
