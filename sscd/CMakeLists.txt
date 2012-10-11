project(sscd)

add_subdirectory(src)
add_subdirectory(icons)

if(WIN32)
  install(FILES sscd.conf mysql_create_script.sql DESTINATION ${BIN_INSTALL_DIR} COMPONENT sscd)
else(WIN32)
  install(FILES sscd.conf mysql_create_script.sql DESTINATION share/sscd COMPONENT sscd)
endif(WIN32)
