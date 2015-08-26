set( webcamsettings_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/simonwebcamconfiguration.cpp
        webcamconfigurationanalyzer.cpp
)

ki18n_wrap_ui( webcamsettings_LIB_SRCS
        simonwebcamconfiguration.ui
)

kconfig_add_kcfg_files(webcamsettings_LIB_SRCS webcamconfiguration.kcfgc)

kde4_add_library(webcamsettings MODULE ${webcamsettings_LIB_SRCS} )
target_link_libraries( webcamsettings ${QT_LIBRARIES}  KF5::KIOCore ${OpenCV_LIBS} simonvision KF5::CoreAddons KF5::ConfigWidgets)

install( TARGETS webcamsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncore )
install( FILES simonwebcamconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simoncore )
