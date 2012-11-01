
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib  )
FIND_PACKAGE( ${SIMONCV_PACKAGE} )
if(${SIMONCV_PACKAGE}_FOUND)
  ADD_DEFINITIONS(-fexceptions)

  set (simonlipdetectionconditionplugin_SRCS
          lipdetectioncondition.cpp
          createlipdetectionconditionwidget.cpp
      )

  kde4_add_ui_files(simonlipdetectionconditionplugin_SRCS
          createlipdetectionconditionwidget.ui
      )

  kde4_add_plugin(simonlipdetectionconditionplugin ${simonlipdetectionconditionplugin_SRCS})


  target_link_libraries(simonlipdetectionconditionplugin ${KDE4_KDEUI_LIBS}
    ${QT_QTXML_LIBRARY} ${KDE4_KDECORE_LIBS} simoncontextdetection simonuicomponents ${OpenCV_LIBS} simonvision )

  install(TARGETS simonlipdetectionconditionplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simonlipdetectionconditionplugin )
  install( FILES simonlipdetectionconditionplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simonlipdetectionconditionplugin )
endif(${SIMONCV_PACKAGE}_FOUND)
