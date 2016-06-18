set(simonrecognition_LIB_SRCS
  recognitioncontrol.cpp
  threadedsslsocket.cpp
  simonsslsocket.cpp
)

set(simonrecognition_LIB_HDRS
  recognitioncontrol.h
)

kconfig_add_kcfg_files(simonrecognition_LIB_SRCS recognitionconfiguration.kcfgc)
kde4_add_library(simonrecognitioncontrol SHARED ${simonrecognition_LIB_SRCS})

generate_export_header(simonrecognitioncontrol)

target_link_libraries(simonrecognitioncontrol ${QT_LIBRARIES}
   simondstreamer simoncontextdetection
  Qt5::Network KF5::KCMUtils
  simonmodelmanagementui simonscenarios simonrecognitionresult simonprogresstracking simonactions)

set_target_properties(simonrecognitioncontrol
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR} DEFINE_SYMBOL MAKE_RECOGNITIONCONTROL_LIB)

install(FILES ${simonrecognition_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/RecognitionControl COMPONENT simondevel)
 
install(TARGETS simonrecognitioncontrol DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)

set( kcm_simonrecognitioncontrol_LIB_SRCS
  networksettings.cpp
  plugin_exporter.cpp
  synchronisationsettings.cpp
)

ki18n_wrap_ui( kcm_simonrecognitioncontrol_LIB_SRCS
  networksettingsdlg.ui
  synchronisationsettings.ui
)

kconfig_add_kcfg_files(kcm_simonrecognitioncontrol_LIB_SRCS recognitionconfiguration.kcfgc)
kde4_add_library(simonrecognitioncontrolkcm MODULE ${kcm_simonrecognitioncontrol_LIB_SRCS})

target_link_libraries( simonrecognitioncontrolkcm   
  simonuicomponents simonrecognitioncontrol
)

install(TARGETS simonrecognitioncontrolkcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simon)
install( FILES simonrecognitionconfig.desktop DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
