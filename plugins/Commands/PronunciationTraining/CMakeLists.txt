set (simonpronunciationtrainingplugin_SRCS 
  pronunciationtrainingcommandmanager.cpp
  pronunciationtrainingconfiguration.cpp 
  pronunciationtraining.cpp 
)

ki18n_wrap_ui(simonpronunciationtrainingplugin_SRCS
  pronunciationtrainingconfigurationdlg.ui
  pronunciationtrainingdlg.ui
)

kde4_add_plugin(simonpronunciationtrainingplugin ${simonpronunciationtrainingplugin_SRCS})

target_link_libraries(simonpronunciationtrainingplugin
  Qt5::Xml simonscenariobase simonlogging simonscenarios
  simonactions simonrecognitionresult)

install(TARGETS simonpronunciationtrainingplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin)
install(FILES simonpronunciationtrainingplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin)
