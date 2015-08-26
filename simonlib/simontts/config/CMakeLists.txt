set(simonttsconfiguration_LIB_SRCS
  ttssettings.cpp
  editrecording.cpp
)

ki18n_wrap_ui(simonttsconfiguration_LIB_SRCS
  simonttsconfigurationdlg.ui
  editrecording.ui
)

kconfig_add_kcfg_files(simonttsconfiguration_LIB_SRCS ttsconfiguration.kcfgc)

kde4_add_library(simonttsconfiguration MODULE ${simonttsconfiguration_LIB_SRCS})
target_link_libraries( simonttsconfiguration simonsound simontts 
  ${QT_LIBRARIES}  KF5::KIOCore)

install(TARGETS simonttsconfiguration DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore)
install(FILES simonttsconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore)
