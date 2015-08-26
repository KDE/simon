set(simoneventhandlertest_SRCS
  eventhandlertest.cpp
)

add_executable(simoneventhandlertest-eventhandler NAME
add_test(simoneventhandlertest simoneventhandlertest)
ecm_mark_as_test(simoneventhandlertest)
  simoneventhandlertest-eventhandler
  ${simoneventhandlertest_SRCS}
)

target_link_libraries(simoneventhandlertest-eventhandler
  KF5::KDELibs4Support  Qt5::Test ${QT_LIBRARIES}
  eventsimulation
)
