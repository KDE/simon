include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES} ../../simonlib
	../../simonlib/speechgui)
set(ksimond_SRCS
   main.cpp
   ksimondview.cpp
 )

kde4_add_executable(ksimond ${ksimond_SRCS})

target_link_libraries(ksimond ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS} speechgui )

install(TARGETS ksimond DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES ksimond.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
