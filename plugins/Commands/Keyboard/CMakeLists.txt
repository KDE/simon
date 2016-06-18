set (simonkeyboardcommandplugin_SRCS 
  keyboardcommandmanager.cpp 
  keyboardconfiguration.cpp
  keyboardtab.cpp
  keyboardset.cpp
  keyboardsetcontainer.cpp
  keyboardbutton.cpp
  keyboardmodifybuttondialog.cpp
  flowlayout.cpp
)

ki18n_wrap_ui(simonkeyboardcommandplugin_SRCS
  keyboardwidget.ui
  keyboardconfigurationdlg.ui
  modifybuttondlg.ui
)

kde4_add_plugin(simonkeyboardcommandplugin ${simonkeyboardcommandplugin_SRCS})

target_link_libraries(simonkeyboardcommandplugin  Qt5::Xml
  simonscenarios simonactions eventsimulation)

install(TARGETS simonkeyboardcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandkeyboardplugin )
install( FILES simonkeyboardcommandplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncommandkeyboardplugin )

install(FILES keyboardsets.xml DESTINATION ${DATA_INSTALL_DIR}/simon/conf COMPONENT simoncommandkeyboardplugin)
