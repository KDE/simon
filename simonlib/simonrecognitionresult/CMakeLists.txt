include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)

set( simonrecognitionresult_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/recognitionresult.cpp
)

set( simonrecognitionresult_LIB_HDRS
  simonrecognitionresult_export.h
  recognitionresult.h
)


kde4_add_library( simonrecognitionresult  SHARED ${simonrecognitionresult_LIB_SRCS} )
target_link_libraries( simonrecognitionresult ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} )


set_target_properties( simonrecognitionresult
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonrecognitionresult_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonrecognitionresult
  COMPONENT simondevel
)

install( TARGETS simonrecognitionresult DESTINATION ${SIMON_LIB_INSTALL_DIR}
	COMPONENT simoncore )
