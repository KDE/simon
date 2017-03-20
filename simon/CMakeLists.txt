project (simon)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(themes)
add_subdirectory(conf)
add_subdirectory(model)
add_subdirectory(scenarios)
add_subdirectory(texts)

install(FILES org.kde.simon.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT simon)
install(FILES org.kde.simon_autostart.desktop DESTINATION ${AUTOSTART_INSTALL_DIR} COMPONENT simon)
install(FILES org.kde.simon.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
