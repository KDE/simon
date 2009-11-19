include(FindPortaudio)

IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -DMAKE_SSCD -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../julius/libsent/include/ ../../julius/libjulius/include/
	../../simonlib ${PORTAUDIO_DEFINITIONS})

set(sscd_SRCS
   main.cpp
   sscdcontrol.cpp
   clientsocket.cpp
   databaseaccess.cpp
   sscqueries.cpp
   mysqlqueries.cpp
 )

kde4_add_app_icon(sscd_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-sscd.png")

kde4_add_executable(sscd ${sscd_SRCS})

target_link_libraries(sscd ${QT_LIBRARIES} sscobjects)

install(TARGETS sscd DESTINATION ${BIN_INSTALL_DIR} COMPONENT sscd )


########### install files ###############

install( FILES sscd.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT sscd )
