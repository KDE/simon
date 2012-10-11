project(simonoid)

set(simonoid_SRCS simonoid.cpp)
kde4_add_ui_files(simonoid_SRCS config.ui)

kde4_add_plugin(plasma_applet_simonoid ${simonoid_SRCS})

target_link_libraries(plasma_applet_simonoid ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})

install(TARGETS plasma_applet_simonoid DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-simonoid.desktop DESTINATION ${SERVICES_INSTALL_DIR})
