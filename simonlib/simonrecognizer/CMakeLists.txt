include(SimonDirs)

# find_package(Julius REQUIRED)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -fexceptions)
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

if(${BackendType} STREQUAL both)
    find_package(Sphinxbase REQUIRED)
    find_package(Pocketsphinx REQUIRED)
    include_directories(${POCKETSPHINX_INCLUDE_DIR} ${SphinxBase_INCLUDE_DIR})
    message("Here:" ${SphinxBase_INCLUDE_DIR})

    set( simonrecognizer_LIB_SRCS
        ${simonrecognizer_LIB_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognitionconfiguration.cpp
    )

    set( simonrecognizer_LIB_HDRS
        ${simonrecognizer_LIB_HDRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognizer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sphinxrecognitionconfiguration.h
    )
endif(${BackendType} STREQUAL both)

kde4_add_library( simonrecognizer SHARED ${simonrecognizer_LIB_SRCS} )

target_link_libraries( simonrecognizer ${QT_QTCORE_LIBRARY} ${KDE4_KDECORE_LIBS} simonrecognitionresult)
#set_target_properties(simonrecognizer PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic  -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Winit-self -Wunreachable-code")

if(${BackendType} STREQUAL both)
    target_link_libraries( simonrecognizer ${POCKETSPHINX_LIBRARIES} ${SphinxBase_LIBRARIES} simonrecognitionresult)
endif(${BackendType} STREQUAL both)

set_target_properties( simonrecognizer
    PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonrecognizer_LIB_HDRS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonrecognizer
    COMPONENT simondevel
)
 
install( TARGETS simonrecognizer
    DESTINATION ${SIMON_LIB_INSTALL_DIR}
    COMPONENT simoncore
)
