set ( simon_model_scripts
		${CMAKE_CURRENT_SOURCE_DIR}/config
		${CMAKE_CURRENT_SOURCE_DIR}/config.global
		${CMAKE_CURRENT_SOURCE_DIR}/config.rc
		${CMAKE_CURRENT_SOURCE_DIR}/global
		${CMAKE_CURRENT_SOURCE_DIR}/gmm1.hed
		${CMAKE_CURRENT_SOURCE_DIR}/gmm2.hed
		${CMAKE_CURRENT_SOURCE_DIR}/gmm3.hed
		${CMAKE_CURRENT_SOURCE_DIR}/global.ded
		${CMAKE_CURRENT_SOURCE_DIR}/mkphones0.led
		${CMAKE_CURRENT_SOURCE_DIR}/mkphones1.led
		${CMAKE_CURRENT_SOURCE_DIR}/mktri.led
		${CMAKE_CURRENT_SOURCE_DIR}/proto
		${CMAKE_CURRENT_SOURCE_DIR}/sil.hed
	)

install( FILES ${simon_model_scripts}  DESTINATION ${DATA_INSTALL_DIR}/simon/scripts COMPONENT simond )
