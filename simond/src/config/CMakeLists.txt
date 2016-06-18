set(simondsettings_LIB_SRCS
  simonduserconfiguration.cpp
  simondnetworkconfiguration.cpp
  plugin_exporter.cpp
)

kconfig_add_kcfg_files(simondsettings_LIB_SRCS simondconfiguration.kcfgc)

ki18n_wrap_ui( simondsettings_LIB_SRCS
  simondnetworkconfiguration.ui
  simonduserconfiguration.ui
)

kde4_add_plugin(simondsettings ${simondsettings_LIB_SRCS})

target_link_libraries(simondsettings Qt5::Core
   KF5::KIOCore simonddatabaseaccess KF5::ConfigCore KF5::ConfigWidgets KF5::WidgetsAddons
  simonuicomponents)

generate_export_header(simondsettings BASE_NAME simondconfiguration)

install(TARGETS simondsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simond)
install(FILES simondconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simond)
