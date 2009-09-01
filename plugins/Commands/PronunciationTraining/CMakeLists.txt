find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonpronunciationtrainingplugin_SRCS 
	pronunciationtrainingcommandmanager.cpp
	pronunciationtrainingconfiguration.cpp 
)

kde4_add_ui_files(simonpronunciationtrainingplugin_SRCS pronunciationtrainingconfigurationdlg.ui)

kde4_add_plugin(simonpronunciationtrainingplugin ${simonpronunciationtrainingplugin_SRCS})

if (${SIMON_EXPERIMENTAL_SCENARIO_SUPPORT})
add_definitions(-DSIMON_SCENARIOS)
endif (${SIMON_EXPERIMENTAL_SCENARIO_SUPPORT})

target_link_libraries(simonpronunciationtrainingplugin ${KDE4_KDEUI_LIBS}
	simonlogging simoncommandpluginbase simonactions simonmodelmanagement)

install(TARGETS simonpronunciationtrainingplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin )
install( FILES simonpronunciationtrainingplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin )
install( FILES simonpronunciationtrainingpluginconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandpronunciationtrainingplugin )
add_subdirectory(po)
