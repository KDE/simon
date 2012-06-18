
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib  )
FIND_PACKAGE( ${SIMONCV_PACKAGE} REQUIRED )

ADD_DEFINITIONS(-fexceptions)

set (simonfacedetectionconditionplugin_SRCS 
	facedetectioncondition.cpp 
        createfacedetectionconditionwidget.cpp
    )

kde4_add_ui_files(simonfacedetectionconditionplugin_SRCS
        createfacedetectionconditionwidget.ui
    )

kde4_add_plugin(simonfacedetectionconditionplugin ${simonfacedetectionconditionplugin_SRCS})


target_link_libraries(simonfacedetectionconditionplugin ${KDE4_KDEUI_LIBS}
  ${QT_QTXML_LIBRARY} ${KDE4_KDECORE_LIBS} simoncontextdetection simonuicomponents  ${OpenCV_LIBS} simonvision )

install(TARGETS simonfacedetectionconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonfacedetectionconditionplugin )
install( FILES simonfacedetectionconditionplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonfacedetectionconditionplugin )
