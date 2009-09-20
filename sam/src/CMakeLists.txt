include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib
	${CMAKE_CURRENT_SOURCE_DIR}/../../julius/libjulius/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../julius/libsent/include
)
set(sam_SRCS
   main.cpp
   samview.cpp
   accuracydisplay.cpp
 )

kde4_add_ui_files( sam_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/main.ui
)


#kde4_add_kcfg_files(sam_SRCS ../config/samconfiguration.kcfgc)

kde4_add_app_icon(sam_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-sam.png")
kde4_add_executable(sam ${sam_SRCS})

target_link_libraries(sam ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS}
	${KDE4_KIO_LIBS} simonmodelcompilation simonmodeltest simonsound simonrecognitionresult)

install(TARGETS sam DESTINATION ${BIN_INSTALL_DIR}  COMPONENT sam)


########### install files ###############

install( FILES sam.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR}  COMPONENT sam)
install(FILES samui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT sam)
