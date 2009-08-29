FILE(GLOB simon_scenarios *.xml) 
install( FILES ${simon_scenarios}  DESTINATION ${DATA_INSTALL_DIR}/simon/scenarios COMPONENT simon )
