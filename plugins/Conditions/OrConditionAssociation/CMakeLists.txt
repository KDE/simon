find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib
${CMAKE_CURRENT_SOURCE_DIR}/../../../simon/src)

set (simonorconditionassociationplugin_SRCS 
	orconditionassociation.cpp 
        createorconditionassociationwidget.cpp
        newassociationcondition.cpp
    )

kde4_add_ui_files(simonorconditionassociationplugin_SRCS
        createorconditionassociationwidget.ui
        newassociationcondition.ui
    )

kde4_add_plugin(simonorconditionassociationplugin ${simonorconditionassociationplugin_SRCS})


target_link_libraries(simonorconditionassociationplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} simoncontextdetection simoncontextui )

install(TARGETS simonorconditionassociationplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonorconditionassociationplugin )
install( FILES simonorconditionassociationplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonorconditionassociationplugin )
