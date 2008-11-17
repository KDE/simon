include(FindJulius)
include(FindPortaudio)
include(FindSoundFile)

find_package(Julius REQUIRED)
find_package(SoundFile REQUIRED)

add_definitions(${KDE_DEFINITIONS} ${QT_DEFINITIONS} -DQT_DEBUG -DDEBUG)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${JULIUS_INCLUDE_DIR}
	../../simonlib )

set(simond_SRCS
   main.cpp
   simondcontrol.cpp
   clientsocket.cpp
   #   databaseaccess.cpp
   synchronisationmanager.cpp
   recognitioncontrol.cpp
   juliuscontrol.cpp
 )

kde4_add_app_icon(simonGeneralSources
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simond.png")

kde4_add_executable(simond ${simond_SRCS})

target_link_libraries(simond ${KDE4_KDEUI_LIBS} ${PORTAUDIO_LIBRARIES} ${SOUNDFILE_LIBRARY}
${QT_LIBRARIES} ${JULIUS_LIBRARIES} speechmodelbase simonmodelcompilation
simonddatabaseaccess)

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
