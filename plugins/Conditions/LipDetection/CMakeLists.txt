set (simonlipdetectionconditionplugin_SRCS
	lipdetectioncondition.cpp
	createlipdetectionconditionwidget.cpp
    )

ki18n_wrap_ui(simonlipdetectionconditionplugin_SRCS
	createlipdetectionconditionwidget.ui
    )

kde4_add_plugin(simonlipdetectionconditionplugin ${simonlipdetectionconditionplugin_SRCS})


target_link_libraries(simonlipdetectionconditionplugin
  Qt5::Xml KF5::KDELibs4Support simoncontextdetection simonuicomponents ${OpenCV_LIBS} simonvision )

install(TARGETS simonlipdetectionconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonlipdetectionconditionplugin )
install( FILES simonlipdetectionconditionplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonlipdetectionconditionplugin )
