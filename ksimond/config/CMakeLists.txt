set(ksimondsettings_LIB_SRCS ksimondsettings.cpp)

kde4_add_kcfg_files(ksimondsettings_LIB_SRCS ksimondconfiguration.kcfgc)

kde4_add_ui_files(ksimondsettings_LIB_SRCS ksimondconfiguration.ui)

kde4_add_plugin(ksimondsettings ${ksimondsettings_LIB_SRCS})

target_link_libraries(ksimondsettings ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS})

install(TARGETS ksimondsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT ksimond)
install(FILES ksimondconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT ksimond)