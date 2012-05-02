find_package(Qwt6 REQUIRED)

include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES}
	${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${Qwt6_INCLUDE_DIR}
	${CMAKE_BINARY_DIR}
)


set(sam_SRCS
   main.cpp
   samui.cpp
   samview.cpp
   conservativetraining.cpp
   samxmlhelper.cpp
   corpusinformationwidget.cpp
   corpusinformation.cpp
   accuracydisplay.cpp
   exporttestresults.cpp
   reporttemplateengine.cpp
   latexreporttemplateengine.cpp
   reportparameters.cpp
   testconfigurationwidget.cpp
   templatevaluelist.cpp
   testresultwidget.cpp
   testresultplotter.cpp
   qwt_bars_item.cpp
 )

kde4_add_ui_files( sam_SRCS
  main.ui
  corpusinformation.ui
  testresult.ui
  conservativetraining.ui
  testconfiguration.ui
  exporttestresultsdlg.ui
)


#kde4_add_kcfg_files(sam_SRCS ../config/samconfiguration.kcfgc)

kde4_add_app_icon(sam_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-sam.png")
kde4_add_executable(sam ${sam_SRCS})

target_link_libraries(sam ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS}
	${KDE4_KIO_LIBS} simonmodelcompilation
	simonmodeltest simonsound ${Qwt6_Qt4_LIBRARY}
	simonrecognitionresult simonscenarioui
)

if(NOT WIN32)
    target_link_libraries(sam rt)
endif(NOT WIN32)

install(TARGETS sam DESTINATION ${BIN_INSTALL_DIR}  COMPONENT sam)


########### install files ###############

install( FILES sam.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR}  COMPONENT sam)
install(FILES samui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT sam)
