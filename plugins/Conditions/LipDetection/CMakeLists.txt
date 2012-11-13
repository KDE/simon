set (simonlipdetectionconditionplugin_SRCS
	lipdetectioncondition.cpp
	createlipdetectionconditionwidget.cpp
    )

kde4_add_ui_files(simonlipdetectionconditionplugin_SRCS
	createlipdetectionconditionwidget.ui
    )

kde4_add_plugin(simonlipdetectionconditionplugin ${simonlipdetectionconditionplugin_SRCS})


target_link_libraries(simonlipdetectionconditionplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} ${KDE4_KDECORE_LIBS} simoncontextdetection simonuicomponents ${OpenCV_LIBS} simonvision )

install(TARGETS simonlipdetectionconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonlipdetectionconditionplugin )
install( FILES simonlipdetectionconditionplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonlipdetectionconditionplugin )
