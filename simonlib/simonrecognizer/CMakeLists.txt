include(SimonDirs)

# find_package(Julius REQUIRED)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonrecognizer_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recognitionconfiguration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognitionconfiguration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusstaticrecognitionconfiguration.cpp
)

set( simonrecognizer_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recognizer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognizer.h
	${CMAKE_CURRENT_SOURCE_DIR}/recognitionconfiguration.h
	${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognitionconfiguration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusstaticrecognitionconfiguration.h
)

kde4_add_library( simonrecognizer SHARED ${simonrecognizer_LIB_SRCS} )

target_link_libraries( simonrecognizer ${QT_QTCORE_LIBRARY} ${KDE4_KDECORE_LIBS} simonrecognitionresult)

set_target_properties( simonrecognizer
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonrecognizer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonrecognizer
  COMPONENT simondevel
)
 
install( TARGETS simonrecognizer DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT
	simoncore )
