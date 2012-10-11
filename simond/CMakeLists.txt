project(simond)

add_subdirectory(src)
add_subdirectory(icons)
add_subdirectory(scripts)

install(FILES default.jconf simond.db DESTINATION ${DATA_INSTALL_DIR}/simond COMPONENT simond)