IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -DMAKE_SIMOND -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

if(${BackendType} STREQUAL both)
  find_package(Sphinxbase REQUIRED)
  find_package(Pocketsphinx REQUIRED)
  include_directories(${POCKETSPHINX_INCLUDE_DIR} ${SphinxBase_INCLUDE_DIR} ../../simonlib)
endif(${BackendType} STREQUAL both)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES})

add_subdirectory( config )


set(simond_SRCS
   main.cpp
   simondcontrol.cpp
   clientsocket.cpp
   synchronisationmanager.cpp
   recognitioncontrolfactory.cpp
   recognitioncontrol.cpp
   juliuscontrol.cpp
 )
if(${BackendType} STREQUAL both)
set(simond_SRCS
    ${simond_SRCS}
    sphinxcontrol.cpp
)
endif(${BackendType} STREQUAL both)

kde4_add_app_icon(simond_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simond.png")

kde4_add_executable(simond ${simond_SRCS})

target_link_libraries(simond ${KDE4_KDECORE_LIBS} ${QT_QTNETWORK_LIBRARY} simonrecognizer
${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} simonscenarios simonmodelcompilation simonddatabaseaccess
simonrecognitionresult simonwav simoncontextadapter simonutils simonrecognizer)

#set_target_properties(simond PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code")

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} COMPONENT simond )


########### install files ###############

install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT simond )
