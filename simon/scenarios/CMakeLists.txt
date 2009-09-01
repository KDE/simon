set (	simon_scenarios 
	${CMAKE_CURRENT_SOURCE_DIR}/webbrowsing
)


install( FILES ${simon_scenarios}  DESTINATION ${DATA_INSTALL_DIR}/simon/scenarios COMPONENT simon )
