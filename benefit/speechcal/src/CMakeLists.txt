include (KDE4Defaults)

add_definitions(-fexceptions)
include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES}
	${KDEPIMLIBS_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/simonlib
	${CMAKE_SOURCE_DIR}
)


set(speechcal_SRCS
   main.cpp
   calendarmodel.cpp
   speechcal.cpp
   speechcalview.cpp
)

kde4_add_ui_files( speechcal_SRCS
  main.ui
)


#kde4_add_kcfg_files(speechcal_SRCS ../config/speechcalconfiguration.kcfgc)

kde4_add_app_icon(speechcal_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-speechcal.png")
kde4_add_executable(speechcal ${speechcal_SRCS})

target_link_libraries(speechcal ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDEPIMLIBS_AKONADI_LIBS} ${KDEPIMLIBS_KCALCORE_LIBS} simonlogging)

install(TARGETS speechcal DESTINATION ${BIN_INSTALL_DIR}  COMPONENT speechcal)


########### install files ###############

install( FILES speechcal.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR}  COMPONENT speechcal)
install(FILES speechcalui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT speechcal)
