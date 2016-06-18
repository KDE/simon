set(sscsettings_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/sscconfiguration.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/plugin_exporter.cpp
)

kconfig_add_kcfg_files(sscsettings_LIB_SRCS sscconfig.kcfgc)

ki18n_wrap_ui(sscsettings_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/sscconfiguration.ui
)

kde4_add_plugin(sscsettings ${sscsettings_LIB_SRCS})

generate_export_header(sscsettings BASE_NAME sscconfiguration)

target_link_libraries(sscsettings ${QT_LIBRARIES} ${QT_LIBRARIES}
   KF5::KIOCore sscdaccess KF5::ConfigWidgets)

install(TARGETS sscsettings DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT ssc)
install(FILES sscconfiguration.desktop DESTINATION ${SERVICES_INSTALL_DIR} COMPONENT ssc)
