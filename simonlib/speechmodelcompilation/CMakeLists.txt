set(simonmodelcompilation_LIB_SRCS
  modelcompilationmanager.cpp
  modelcompilationmanagerhtk.cpp
  modelcompilationmanagersphinx.cpp
  modelcompilationadapter.cpp
  modelcompilationadapterhtk.cpp
  modelcompilationadaptersphinx.cpp
  modelcompilersphinx.cpp
  modelcompilerhtk.cpp
  modelcompiler.cpp
)


set(simonmodelcompilation_LIB_HDRS
  simonmodelcompilationmanagement_export.h
  modelcompilationmanager.h
  modelcompilationmanagerhtk.h
  modelcompilationmanagersphinx.h
  modelcompiler.h
  modelcompilerhtk.h
  modelcompilersphinx.h
  modelcompilation.h
  modelcompilationadapter.h
  modelcompilationadapterhtk.h
  modelcompilationadaptersphinx.h
)

kde4_add_library(simonmodelcompilation SHARED ${simonmodelcompilation_LIB_SRCS})
target_link_libraries(simonmodelcompilation ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} 
  ${KDE4_KDECORE_LIBS} simonlogging simonscenarios simonscenariobase simonutils)

set_target_properties(simonmodelcompilation
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonmodelcompilation_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelcompilation
  COMPONENT simonddevel
)
 
install(TARGETS simonmodelcompilation DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )

add_subdirectory(config)