include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib
	${CMAKE_CURRENT_SOURCE_DIR}/../..
)


set(afaras_SRCS
   main.cpp
   afarasview.cpp
 )

kde4_add_ui_files( afaras_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/main.ui
)

#kde4_add_kcfg_files(afaras_SRCS afarasconfiguration.kcfgc)

kde4_add_app_icon(afaras_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-afaras.png")
kde4_add_executable(afaras ${afaras_SRCS})

target_link_libraries(afaras ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS}
	${KDE4_KIO_LIBS} simonsound)

install(TARGETS afaras DESTINATION ${BIN_INSTALL_DIR} COMPONENT afaras)


########### install files ###############

install( FILES afaras.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR}  COMPONENT afaras)
install(FILES afarasui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT afaras)
