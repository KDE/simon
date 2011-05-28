find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonprocessopenedconditionplugin_SRCS 
	processopenedcondition.cpp 
    )

kde4_add_plugin(simonprocessopenedconditionplugin ${simonprocessopenedconditionplugin_SRCS})


target_link_libraries(simonprocessopenedconditionplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simoncontextdetection)

install(TARGETS simonprocessopenedconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonprocessopenedconditionplugin )
install( FILES simonprocessopenedconditionplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonprocessopenedconditionplugin )
