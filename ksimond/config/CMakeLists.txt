set(ksimondsettings_LIB_SRCS ksimondsettings.cpp)

kconfig_add_kcfg_files(ksimondsettings_LIB_SRCS ksimondconfiguration.kcfgc)

ki18n_wrap_ui(ksimondsettings_LIB_SRCS ksimondconfiguration.ui)

kde4_add_library(ksimondsettings ${ksimondsettings_LIB_SRCS})

target_link_libraries(ksimondsettings ${QT_LIBRARIES} Qt5::Core Qt5::Network KF5::ConfigWidgets KF5::KDELibs4Support)

install(TARGETS ksimondsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT ksimond)
install(FILES ksimondconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT ksimond)
