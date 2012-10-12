FILE(GLOB simon_configs *.xml) 
install(FILES ${simon_configs}  DESTINATION ${DATA_INSTALL_DIR}/simon/conf COMPONENT simon)
