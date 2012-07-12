include(SimonDirs)

# find_package(Julius REQUIRED)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -fexceptions)
include(KDE4Defaults)

#if(USE_SPHINX)
find_package(Sphinxbase REQUIRED)
find_package(Pocketsphinx REQUIRED)
include_directories(${POCKETSPHINX_INCLUDE_DIR} ${SPHINXBASE_INCLUDE_DIR})
#endif(USE_SPHINX)


set( simonrecognizer_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognizer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recognitionconfiguration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognitionconfiguration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognitionconfiguration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusstaticrecognitionconfiguration.cpp
)

set( simonrecognizer_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/recognizer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognizer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognizer.h
	${CMAKE_CURRENT_SOURCE_DIR}/recognitionconfiguration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognitionconfiguration.h
	${CMAKE_CURRENT_SOURCE_DIR}/juliusrecognitionconfiguration.h
        ${CMAKE_CURRENT_SOURCE_DIR}/juliusstaticrecognitionconfiguration.h
)

kde4_add_library( simonrecognizer SHARED ${simonrecognizer_LIB_SRCS} )

target_link_libraries( simonrecognizer ${QT_QTCORE_LIBRARY} ${KDE4_KDECORE_LIBS} ${POCKETSPHINX_LIBRARIES} sphinxad sphinxbase simonrecognitionresult)
#set_target_properties(simonrecognizer PROPERTIES COMPILE_FLAGS "-std=c++0x -Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code")


set_target_properties( simonrecognizer
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonrecognizer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonrecognizer
  COMPONENT simondevel
)
 
install( TARGETS simonrecognizer DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT
	simoncore )
