ENABLE_TESTING()

find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtScript REQUIRED)
include (${QT_USE_FILE})

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(USE_PLASMA true CACHE BOOL "Use the plasma data engines in the dialog plugin")

if (USE_PLASMA)
add_definitions(-DUSE_PLASMA)
endif (USE_PLASMA)

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simondialogcommandplugin_SRCS 
	dialogcommandmanager.cpp 
	dialogconfiguration.cpp
	dialogcommand.cpp
	createdialogcommandwidget.cpp

  visualdialogview.cpp
  dialogstate.cpp
  dialogtext.cpp
  dialogtextparser.cpp

  dialogcommandbutton.cpp
  createtransitiondialog.cpp

  dialogtemplateoptions.cpp
  createtemplateoptiondialog.cpp

  createboundvaluedialog.cpp
 
  boundvalue.cpp
  dialogboundvalues.cpp
  staticboundvalue.cpp
  scriptboundvalue.cpp
    )

if (USE_PLASMA)
set (simondialogcommandplugin_SRCS 
  ${simondialogcommandplugin_SRCS}
  plasmaboundvalue.cpp
  )
endif(USE_PLASMA)

kde4_add_ui_files(simondialogcommandplugin_SRCS 
	dialogwidget.ui
	dialogconfigurationdlg.ui
	createdialogcommandwidget.ui
	createtransitiondlg.ui
  createtemplateoptiondialog.ui
  createboundvalue.ui
    )

kde4_add_plugin(simondialogcommandplugin ${simondialogcommandplugin_SRCS})


if (USE_PLASMA)
target_link_libraries(simondialogcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_LIBRARIES}	${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS} simonscenarios simonscenariobase simonactions )
else (USE_PLASMA)
target_link_libraries(simondialogcommandplugin ${KDE4_KDEUI_LIBS}
  ${QT_LIBRARIES}	${KDE4_KIO_LIBS} simonscenarios simonscenariobase simonactions )
endif(USE_PLASMA)

install(TARGETS simondialogcommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommanddialogplugin )
install( FILES simondialogcommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommanddialogplugin )
add_subdirectory(po)

########
# Tests
########

add_subdirectory(tests)


