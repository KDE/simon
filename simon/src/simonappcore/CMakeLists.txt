set(simonappcore_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/simoncontrol.cpp
)

set(simonappcore_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/simoncontrol.h
  ${CMAKE_CURRENT_SOURCE_DIR}/simonappcore_export.h
)

kconfig_add_kcfg_files(simonappcore_LIB_SRCS coreconfiguration.kcfgc)
kde4_add_library(simonappcore SHARED ${simonappcore_LIB_SRCS} )

target_link_libraries(simonappcore
  ${QT_LIBRARIES} KF5::I18n KF5::Archive KF5::ConfigWidgets KF5::KDELibs4Support
  simoninfo simonlogging simonrecognitioncontrol simonactions
  simonmodelmanagementui simonscenarios simonrecognitionresult
  simonsound simontts simoncontextdetection
)

generate_export_header(simonappcore)

set_target_properties(simonappcore PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR})

install(FILES ${simonappcore_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/AppCore COMPONENT simondevel)
install(TARGETS simonappcore DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)

set( kcm_simonappcore_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/generalsettings.cpp)
ki18n_wrap_ui(kcm_simonappcore_LIB_SRCS generalsettingsdlg.ui)
kconfig_add_kcfg_files(kcm_simonappcore_LIB_SRCS coreconfiguration.kcfgc)

kde4_add_library(simonappcorekcm MODULE ${kcm_simonappcore_LIB_SRCS} )
target_link_libraries( simonappcorekcm KF5::Archive KF5::ConfigCore KF5::ConfigGui KF5::ConfigWidgets KF5::KDELibs4Support)

install(TARGETS simonappcorekcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simon)
install(FILES simongeneralconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT simon)
