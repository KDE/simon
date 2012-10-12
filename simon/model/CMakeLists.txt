set(simon_model_files
  ${CMAKE_CURRENT_SOURCE_DIR}/phonemes
  ${CMAKE_CURRENT_SOURCE_DIR}/prompts
)

install(FILES ${simon_model_files} DESTINATION ${DATA_INSTALL_DIR}/simon/model COMPONENT simon)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/shadowvocabulary.xml DESTINATION ${DATA_INSTALL_DIR}/simon COMPONENT simon)
