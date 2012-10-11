set(simonmodeltest_LIB_SRCS
  modeltest.cpp
  juliusmodeltest.cpp
  recognizerresult.cpp
  fileresultmodel.cpp
  testresult.cpp
  testresultmodel.cpp
  testresultinstance.cpp
  testresultleaf.cpp
)

set(simonmodeltest_LIB_HDRS
  simonmodeltest_export.h
  modeltest.h
  juliusmodeltest.h
  fileresultmodel.h
  recognizerresult.h
)

if(${BackendType} STREQUAL both)
  set(simonmodeltest_LIB_SRCS ${simonmodeltest_LIB_SRCS} sphinxmodeltest.cpp)
  set(simonmodeltest_LIB_HDRS ${simonmodeltest_LIB_HDRS} sphinxmodeltest.h)
endif()

kde4_add_library(simonmodeltest SHARED ${simonmodeltest_LIB_SRCS})
target_link_libraries(simonmodeltest ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} simonlogging 
  simonrecognitionresult simonrecognizer)


set_target_properties(simonmodeltest
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonmodeltest_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonmodeltest
  COMPONENT simonddevel
)

install(TARGETS simonmodeltest DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT sam)

add_subdirectory(test)