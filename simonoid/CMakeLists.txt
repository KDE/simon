project(simonoid)

set(simonoid_SRCS simonoid.cpp)
ki18n_wrap_ui(simonoid_SRCS config.ui)

kde4_add_plugin(plasma_applet_simonoid ${simonoid_SRCS})

target_link_libraries(plasma_applet_simonoid KF5::Plasma )

install(TARGETS plasma_applet_simonoid DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-simonoid.desktop DESTINATION ${SERVICES_INSTALL_DIR})
