set (simonpronunciationtrainingplugin_SRCS 
  pronunciationtrainingcommandmanager.cpp
  pronunciationtrainingconfiguration.cpp 
  pronunciationtraining.cpp 
)

kde4_add_ui_files(simonpronunciationtrainingplugin_SRCS 
  pronunciationtrainingconfigurationdlg.ui
  pronunciationtrainingdlg.ui
)

kde4_add_plugin(simonpronunciationtrainingplugin ${simonpronunciationtrainingplugin_SRCS})

target_link_libraries(simonpronunciationtrainingplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simonscenariobase simonlogging simonscenarios 
  simonactions simonrecognitionresult)

install(TARGETS simonpronunciationtrainingplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin)
install(FILES simonpronunciationtrainingplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin)
