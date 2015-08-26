set (simonfacedetectionconditionplugin_SRCS
	facedetectioncondition.cpp
	createfacedetectionconditionwidget.cpp
    )

ki18n_wrap_ui(simonfacedetectionconditionplugin_SRCS
	createfacedetectionconditionwidget.ui
    )

kde4_add_plugin(simonfacedetectionconditionplugin ${simonfacedetectionconditionplugin_SRCS})


target_link_libraries(simonfacedetectionconditionplugin
  Qt5::Xml KF5::KDELibs4Support simoncontextdetection simonuicomponents ${OpenCV_LIBS} simonvision )

install(TARGETS simonfacedetectionconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonfacedetectionconditionplugin )
install( FILES simonfacedetectionconditionplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonfacedetectionconditionplugin )

