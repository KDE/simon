set ( simon_model_files
		${CMAKE_CURRENT_SOURCE_DIR}/hmmdefs
		${CMAKE_CURRENT_SOURCE_DIR}/lexicon
		${CMAKE_CURRENT_SOURCE_DIR}/model.dfa
		${CMAKE_CURRENT_SOURCE_DIR}/model.dict
		${CMAKE_CURRENT_SOURCE_DIR}/model.grammar
		${CMAKE_CURRENT_SOURCE_DIR}/model.voca
		${CMAKE_CURRENT_SOURCE_DIR}/phonemes
		${CMAKE_CURRENT_SOURCE_DIR}/prompts
		${CMAKE_CURRENT_SOURCE_DIR}/shadowlexicon
		${CMAKE_CURRENT_SOURCE_DIR}/shadow.voca
		${CMAKE_CURRENT_SOURCE_DIR}/tiedlist
		${CMAKE_CURRENT_SOURCE_DIR}/tree1.hed
		${CMAKE_CURRENT_SOURCE_DIR}/wav_config
	)

install( FILES ${simon_model_files}  DESTINATION ${DATA_INSTALL_DIR}/simon/model )

add_subdirectory(scripts)