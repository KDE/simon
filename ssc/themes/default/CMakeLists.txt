FILE(GLOB ssc_pngs *.png) 
install(FILES ${ssc_pngs} DESTINATION ${DATA_INSTALL_DIR}/ssc/themes/default COMPONENT ssc)
