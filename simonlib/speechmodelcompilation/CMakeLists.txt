
include(SimonDirs)
#include(simonutils)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -D__INTERLOCKED_DECLARED -DTSPHINX -fexceptions)
include(KDE4Defaults)

set( simonmodelcompilation_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanagerhtk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationmanagersphinx.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadapter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadapterhtk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilationadaptersphinx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilersphinx.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompilerhtk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/modelcompiler.cpp
)


set( simonmodelcompilation_LIB_HDRS
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

kde4_add_library( simonmodelcompilation  SHARED ${simonmodelcompilation_LIB_SRCS} )
target_link_libraries( simonmodelcompilation ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${KDE4_KDECORE_LIBS} simonlogging simonscenarios simonscenariobase simonutils)

#set_target_properties(simonmodelcompilation PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code")
#set_target_properties(my_target PROPERTIES COMPILE_FLAGS "-g -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -Winit-self -Wstrict-overflow=5 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wsign-promo -ansi -fno-strict-overflow -fstrict-aliasing -Wctor-dtor-privacy -Wnon-virtual-dtor -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wfloat-equal -Wundef -Wshadow -Wconversion -Wlogical-op -Wmissing-declarations -Winline -Wvla")
set_target_properties( simonmodelcompilation
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simonmodelcompilation_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelcompilation
  COMPONENT simonddevel
)
 
install( TARGETS simonmodelcompilation DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simond )

add_subdirectory(config)
