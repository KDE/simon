
include(KDE4Defaults)
include(KDE4Macros)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-fexceptions)
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDEPIMLIBS_INCLUDE_DIRS}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib)

set (simonakonadicommandplugin_SRCS 
	akonadicommandmanager.cpp 
	akonadiconfiguration.cpp
	akonadicommand.cpp
	timeselector.cpp
	createakonadicommandwidget.cpp
	scheduleitem.cpp
	alarmscheduleitem.cpp
	commandscheduleitem.cpp
	dialogrunner.cpp
    )

kde4_add_ui_files(simonakonadicommandplugin_SRCS 
	akonadiconfigurationdlg.ui
	createakonadicommandwidget.ui
	timeselector.ui
    )

kde4_add_plugin(simonakonadicommandplugin ${simonakonadicommandplugin_SRCS})


target_link_libraries(simonakonadicommandplugin ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY}
	${KDEPIMLIBS_AKONADI_LIBS} ${KDEPIMLIBS_KCALCORE_LIBS} simonscenariobase simonscenarios simonlogging simonactions
	simondialogengine simondialogengineconfui simondialogenginegraphicalview simondialogenginettsview)

install(TARGETS simonakonadicommandplugin DESTINATION ${PLUGIN_INSTALL_DIR} COMPONENT simoncommandakonadiplugin )
install( FILES simonakonadicommandplugin.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simoncommandakonadiplugin )

# install(FILES akonadisets.xml DESTINATION
# 	${DATA_INSTALL_DIR}/simon/conf COMPONENT simoncommandakonadiplugin)
