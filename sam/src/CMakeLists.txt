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
  sphinxtestconfigurationwidget.cpp
  juliustestconfigurationwidget.cpp
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
  sphinxtestconfigurationwidget.ui
  juliustestconfigurationwidget.ui
  exporttestresultsdlg.ui
)

kde4_add_app_icon(sam_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-sam.png")
kde4_add_executable(sam ${sam_SRCS})

target_link_libraries(sam ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KUTILS_LIBS}
  ${KDE4_KIO_LIBS} simonmodelcompilation simonmodeltest simonsound 
  ${Qwt6_Qt4_LIBRARY} simonrecognitionresult simonscenarioui simonutils
)

if(NOT WIN32 AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  target_link_libraries(sam rt)
endif()

install(TARGETS sam DESTINATION ${BIN_INSTALL_DIR} COMPONENT sam)

install(FILES org.kde.sam.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT sam)
install(FILES samui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT sam)
