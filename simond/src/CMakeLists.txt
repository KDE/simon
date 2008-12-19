include(FindJulius)
include(FindPortaudio)

find_package(Julius REQUIRED)

IF(UNIX)
	include(FindSoundFile)
	find_package(SoundFile REQUIRED)
ENDIF(UNIX)
IF(WIN32)
#	include(FindDirectSound)
#	find_package(DirectSound REQUIRED)
	add_definitions(-D__INTERLOCKED_DECLARED -DMAKE_SIMOND -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${JULIUS_INCLUDE_DIR}
	../../simonlib ${PORTAUDIO_DEFINITIONS})

set(simond_SRCS
   main.cpp
   simondcontrol.cpp
   clientsocket.cpp
   synchronisationmanager.cpp
   recognitioncontrol.cpp
   juliuscontrol.cpp
 )

kde4_add_app_icon(simonGeneralSources
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simond.png")

kde4_add_executable(simond ${simond_SRCS})

IF(UNIX)
target_link_libraries(simond ${KDE4_KDECORE_LIBS} ${PORTAUDIO_LIBRARIES} ${SOUNDFILE_LIBRARY}
${QT_LIBRARIES} ${JULIUS_LIBRARIES} speechmodelbase simonmodelcompilation simonddatabaseaccess)
ENDIF(UNIX)
IF(WIN32)
target_link_libraries(simond ${KDE4_KDECORE_LIBS} ${PORTAUDIO_LIBRARIES}# ${DIRECTSOUND_LIBRARY}
${QT_LIBRARIES} ${JULIUS_LIBRARIES} speechmodelbase simonmodelcompilation simonddatabaseaccess wsock32 dsound winmm)
ENDIF(WIN32)

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
