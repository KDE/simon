set(simonrecognition_LIB_SRCS
  recognitioncontrol.cpp
  threadedsslsocket.cpp
  simonsslsocket.cpp
)

set(simonrecognition_LIB_HDRS
  recognitioncontrol.h
)

kde4_add_kcfg_files(simonrecognition_LIB_SRCS recognitionconfiguration.kcfgc)
kde4_add_library(simonrecognitioncontrol SHARED ${simonrecognition_LIB_SRCS})

target_link_libraries(simonrecognitioncontrol ${QT_LIBRARIES}
  ${KDE4_KDEUI_LIBS} simondstreamer simoncontextdetection
  ${QT_QTNETWORK_LIBRARY}
  simonmodelmanagementui simonscenarios simonrecognitionresult simonprogresstracking)

set_target_properties(simonrecognitioncontrol
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR} DEFINE_SYMBOL MAKE_RECOGNITIONCONTROL_LIB)

install(FILES ${simonrecognition_LIB_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/simon/RecognitionControl COMPONENT simondevel)
 
install(TARGETS simonrecognitioncontrol DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon)

set( kcm_simonrecognitioncontrol_LIB_SRCS
  networksettings.cpp
  plugin_exporter.cpp
  synchronisationsettings.cpp
)

kde4_add_ui_files( kcm_simonrecognitioncontrol_LIB_SRCS
  networksettingsdlg.ui
  synchronisationsettings.ui
)

kde4_add_kcfg_files(kcm_simonrecognitioncontrol_LIB_SRCS recognitionconfiguration.kcfgc)
kde4_add_plugin(simonrecognitioncontrolkcm ${kcm_simonrecognitioncontrol_LIB_SRCS})

target_link_libraries( simonrecognitioncontrolkcm  ${KDE4_KUTILS_LIBS} ${KDE4_KDEUI_LIBS}
  simonuicomponents simonrecognitioncontrol
)

install(TARGETS simonrecognitioncontrolkcm DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simon)
install( FILES simonrecognitionconfig.desktop DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simon )
