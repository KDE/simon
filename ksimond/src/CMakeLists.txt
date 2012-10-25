set(ksimond_SRCS
  main.cpp
  ksimondview.cpp
)

kde4_add_kcfg_files(ksimond_SRCS ../config/ksimondconfiguration.kcfgc)

kde4_add_app_icon(ksimond_SRCS "../icons/hi*-app-ksimond.png")
kde4_add_executable(ksimond ${ksimond_SRCS})

target_link_libraries(ksimond ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS}
  simonuicomponents simoninfo)

install(TARGETS ksimond DESTINATION ${BIN_INSTALL_DIR} COMPONENT ksimond)

install(FILES ksimond.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT ksimond)
install(FILES ksimond_autostart.desktop DESTINATION ${AUTOSTART_INSTALL_DIR} COMPONENT ksimond)
