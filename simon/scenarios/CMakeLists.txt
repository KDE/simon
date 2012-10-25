set(simon_scenarios ${CMAKE_CURRENT_SOURCE_DIR}/general)

install(FILES ${simon_scenarios} DESTINATION ${DATA_INSTALL_DIR}/simon/scenarios COMPONENT simon)
install(FILES simonscenarios.knsrc DESTINATION ${CONFIG_INSTALL_DIR} COMPONENT simon)
