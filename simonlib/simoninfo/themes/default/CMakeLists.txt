FILE(GLOB simoninfo_pngs *.png) 
install(FILES ${simoninfo_pngs}  DESTINATION ${DATA_INSTALL_DIR}/simoninfo/themes/default COMPONENT simoncore)
