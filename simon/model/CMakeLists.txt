set(simon_model_files
  ${CMAKE_CURRENT_SOURCE_DIR}/phonemes
)

install(FILES ${simon_model_files} DESTINATION ${DATA_INSTALL_DIR}/simon/model COMPONENT simon)
