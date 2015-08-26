set(simonrecognizer_LIB_SRCS
  juliusrecognizer.cpp
  recognitionconfiguration.cpp
  juliusrecognitionconfiguration.cpp
  juliusstaticrecognitionconfiguration.cpp
)

set(simonrecognizer_LIB_HDRS
  recognizer.h
  juliusrecognizer.h
  recognitionconfiguration.h
  juliusrecognitionconfiguration.h
  juliusstaticrecognitionconfiguration.h
)

if(${BackendType} STREQUAL both)
  set(simonrecognizer_LIB_SRCS
    ${simonrecognizer_LIB_SRCS}
    sphinxrecognizer.cpp
    sphinxrecognitionconfiguration.cpp
  )

  set(simonrecognizer_LIB_HDRS
    ${simonrecognizer_LIB_HDRS}
    sphinxrecognizer.h
    sphinxrecognitionconfiguration.h
  )
endif()

kde4_add_library(simonrecognizer SHARED ${simonrecognizer_LIB_SRCS})

target_link_libraries(simonrecognizer Qt5::Core KF5::KDELibs4Support simonrecognitionresult)

if(${BackendType} STREQUAL both)
  target_link_libraries(simonrecognizer ${POCKETSPHINX_LIBRARIES} ${SphinxBase_LIBRARIES} simonrecognitionresult)
endif(${BackendType} STREQUAL both)

set_target_properties(simonrecognizer
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonrecognizer_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonrecognizer
  COMPONENT simondevel
)
 
install(TARGETS simonrecognizer DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
