include(FindPortaudio)

IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -DMAKE_SIMOND -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../julius/libsent/include/ ../../julius/libjulius/include/
	../../simonlib ${PORTAUDIO_DEFINITIONS})

set(simond_SRCS
   main.cpp
   simondcontrol.cpp
   clientsocket.cpp
   synchronisationmanager.cpp
   recognitioncontrol.cpp
   juliuscontrol.cpp
 )

kde4_add_app_icon(simond_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simond.png")

kde4_add_executable(simond ${simond_SRCS})

if (${SIMON_EXPERIMENTAL_SCENARIO_SUPPORT})
add_definitions(-DSIMON_SCENARIOS)
endif (${SIMON_EXPERIMENTAL_SCENARIO_SUPPORT})

target_link_libraries(simond ${KDE4_KDECORE_LIBS} ${PORTAUDIO_LIBRARIES}
${QT_LIBRARIES} simonscenarios simonmodelcompilation simonddatabaseaccess
simonrecognitionresult julius sent simonmodelcompilationadapter)

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} COMPONENT simond )


########### install files ###############

install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT simond )
