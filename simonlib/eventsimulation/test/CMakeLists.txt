set(simoneventhandlertest_SRCS
  eventhandlertest.cpp
)

kde4_add_unit_test(simoneventhandlertest-eventhandler TESTNAME
  simoneventhandlertest-eventhandler
  ${simoneventhandlertest_SRCS}
)

target_link_libraries(simoneventhandlertest-eventhandler
  ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTTEST_LIBRARY} ${QT_LIBRARIES} 
  eventsimulation
)
