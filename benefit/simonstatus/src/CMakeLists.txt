
include (KDE4Defaults)

include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES}
	${KDEPIMLIBS_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../simonlib
	${CMAKE_CURRENT_SOURCE_DIR}/../../..
)


set(simonstatus_SRCS
   main.cpp
   simonstatuswidget.cpp
 )


kde4_add_app_icon(simonstatus_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simonstatus.png")

qt4_add_dbus_adaptor( simonstatus_SRCS  org.simon-listens.SimonStatusWidget.xml
  simonstatuswidget.h SimonStatusWidget)

kde4_add_executable(simonstatus ${simonstatus_SRCS})

target_link_libraries(simonstatus ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS})

install(TARGETS simonstatus DESTINATION ${BIN_INSTALL_DIR}  COMPONENT simonstatus)


########### install files ###############

install( FILES simonstatus.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR}  COMPONENT simonstatus)
