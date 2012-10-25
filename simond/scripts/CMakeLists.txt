set(simon_model_scripts
  config
  config.global
  config.rc
  global
  gmm1.hed
  gmm2.hed
  gmm3.hed
  global.ded
  mkphones0.led
  mkphones1.led
  mktri.led
  proto
  regtree.hed
  sil.hed
  tree1.hed
  wav_config
)

install(FILES ${simon_model_scripts} DESTINATION ${DATA_INSTALL_DIR}/simon/scripts COMPONENT simond)
