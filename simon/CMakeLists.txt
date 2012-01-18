project (simon)
include(FindKDE4)



add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(icons)
add_subdirectory(themes)
add_subdirectory(conf)
add_subdirectory(model)
add_subdirectory(scenarios)
add_subdirectory(texts)

install(FILES simon.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT simon)
install(FILES simon_autostart.desktop DESTINATION ${AUTOSTART_INSTALL_DIR} COMPONENT simon)
