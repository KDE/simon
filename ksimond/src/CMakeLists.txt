set(ksimond_SRCS
  main.cpp
  ksimondview.cpp
)

kconfig_add_kcfg_files(ksimond_SRCS ../config/ksimondconfiguration.kcfgc)

file(GLOB ICONS_SRCS "../icons/hi*-app-ksimond.png")
kde4_add_app_icon(ksimond_SRCS ICONS ${ICONS_SRCS})
add_executable(ksimond ${ksimond_SRCS})

target_link_libraries(ksimond KF5::KCMUtils Qt5::Core
  simonuicomponents simoninfo)

install(TARGETS ksimond ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES org.kde.ksimond.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT ksimond)
install(FILES org.kde.ksimond_autostart.desktop DESTINATION ${AUTOSTART_INSTALL_DIR} COMPONENT ksimond)
