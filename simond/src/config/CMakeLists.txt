set(simondsettings_LIB_SRCS
  simonduserconfiguration.cpp
  simondnetworkconfiguration.cpp
  plugin_exporter.cpp
)

kde4_add_kcfg_files(simondsettings_LIB_SRCS simondconfiguration.kcfgc)

kde4_add_ui_files( simondsettings_LIB_SRCS
  simondnetworkconfiguration.ui
  simonduserconfiguration.ui
)

kde4_add_plugin(simondsettings ${simondsettings_LIB_SRCS})

target_link_libraries(simondsettings ${QT_LIBRARIES} ${QT_LIBRARIES}
  ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} simonddatabaseaccess
  simonuicomponents)
 
install(TARGETS simondsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simond)
install(FILES simondconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simond)
