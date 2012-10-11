set(simonttsconfiguration_LIB_SRCS
  ttssettings.cpp
  editrecording.cpp
)

kde4_add_ui_files(simonttsconfiguration_LIB_SRCS
  simonttsconfigurationdlg.ui
  editrecording.ui
)

kde4_add_kcfg_files(simonttsconfiguration_LIB_SRCS ttsconfiguration.kcfgc)

kde4_add_plugin(simonttsconfiguration ${simonttsconfiguration_LIB_SRCS})
target_link_libraries( simonttsconfiguration simonsound simontts 
  ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

install(TARGETS simonttsconfiguration DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore)
install(FILES simonttsconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore)
