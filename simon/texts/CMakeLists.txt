FILE(GLOB simon_texts *.xml) 
install(FILES ${simon_texts} DESTINATION ${DATA_INSTALL_DIR}/simon/texts COMPONENT simon)