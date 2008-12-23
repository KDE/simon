FILE(GLOB ai_aimls *.aiml) 
install( FILES ${ai_aimls}  DESTINATION ${DATA_INSTALL_DIR}/ai/aimls/Alice COMPONENT simoncommandaipluginaimlsalice )
