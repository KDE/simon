FILE(GLOB simon_pngs *.png) 
install( FILES ${simon_pngs}  DESTINATION
	${DATA_INSTALL_DIR}/simon/themes/default/l10n/de COMPONENT simon )
