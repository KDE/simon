set(simond_SRCS
   simond.cpp
   main.cpp
   simondview.cpp
 )

kde4_add_ui_files(simond_SRCS simondview_base.ui prefs_base.ui)

kde4_add_kcfg_files(simond_SRCS settings.kcfgc )

kde4_add_executable(simond ${simond_SRCS})

target_link_libraries(simond ${KDE4_KDEUI_LIBS} )

install(TARGETS simond DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES simond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES simond.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES simondui.rc  DESTINATION  ${DATA_INSTALL_DIR}/simond )
