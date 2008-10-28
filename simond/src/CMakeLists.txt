

set(simond_SRCS
   main.cpp
   simondcontrol.cpp
   clientsocket.cpp
   databaseaccess.cpp
   synchronisationmanager.cpp
#    modelcompilationmanager.cpp
   recognitioncontrol.cpp
   juliuscontrol.cpp
 )


kde4_add_kcfg_files(simond_SRCS settings.kcfgc )

kde4_add_executable(simond ${simond_SRCS})

target_link_libraries(simond ${KDE4_KDEUI_LIBS}  ${QT_LIBRARIES} speechmodelbase simonmodelcompilation )

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES simond.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
